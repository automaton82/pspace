#include "MapCollisions.h"

#include <float.h>

#include "MathUtil.h"
using namespace MathUtil;

void calcDistanceStep(const TileMap& map, double time, const Vector& vel, Vector& increments)
{
	//ignore z component
	if(fabs(vel.x_) > fabs(vel.y_))	
	{						//larger increment is in x
		increments.x_ = min(map.getTileWidth() - TOLERANCE, fabs(vel.x_*time)) * sign(vel.x_);
		increments.y_ = vel.y_/vel.x_ * increments.x_;
	}
	else
	{						//larger increment is in y
		increments.y_ = min(map.getTileHeight() - TOLERANCE, fabs(vel.y_*time)) * sign(vel.y_);
		increments.x_ = vel.x_/vel.y_ * increments.y_;
	}
}

bool checkMapCollision(const TileMap& map, double timestep, const Vector& oldPosition, const Vector& oldVelocity, Vector* collidePosition, double* collideTime)
{
	Vector finalPos = oldPosition + oldVelocity*timestep;	//next position assuming no collision
	Vector finalVel = oldVelocity;						//next velocity assuming no collision
	Vector moveDist = abs(finalPos - oldPosition);		//total movement
	
	Vector incs;
	calcDistanceStep(map, timestep, oldVelocity, incs);	
	double incRatio = incs.y_ / incs.x_;
		
	double xDist, xTime;
	double yDist, yTime;

	Uint tx, ty;
	double earliestCollision = 0;		//TODO: 0 retval means no collision - could that cause problems?

	Vector newPosition = oldPosition;

	int ct=0;

    while(moveDist.x_ + moveDist.y_ > TOLERANCE)
	{														//check every increment (largest is a whole tile)
		xDist = min(fabs(moveDist.x_), map.getTileWidth() - TOLERANCE);		//move maximum of 1 tile
			
		if(oldVelocity.x_ == 0)		//calculate time to tile
			xTime = DBL_MAX;
		else
			xTime = fabs(xDist / oldVelocity.x_);
				
		yDist = min(fabs(moveDist.y_), map.getTileHeight() - TOLERANCE);
		
		if(oldVelocity.y_ == 0)
			yTime = DBL_MAX;
		else
			yTime = fabs(yDist / oldVelocity.y_);
		
												
		if(xTime <= yTime)			//check sooner collision
		{   
			newPosition.x_ += incs.x_;	//increment x first
			moveDist.x_ -= fabs(incs.x_);

			map.objectToTileCoords(newPosition, &tx, &ty);	//next tile position (could be the same tile)
			if(map.isTileCollidable(tx, ty))
			{
				newPosition.x_ = round(newPosition.x_, map.getTileWidth()) - sign(oldVelocity.x_)*TOLERANCE;
										
				earliestCollision = xTime;
				break;			
			}

			newPosition.y_ += incs.y_;	//increment y second
			moveDist.y_ -= fabs(incs.y_);

			map.objectToTileCoords(newPosition, &tx, &ty);	//next tile position (could be the same tile)
			if(map.isTileCollidable(tx, ty))				//crossed tile boundary in y direction
			{	
				newPosition.y_ = round(newPosition.y_, map.getTileHeight()) - sign(oldVelocity.y_)*TOLERANCE;
			
				earliestCollision = yTime;
				break;				
			}
		}
		else
		{
			newPosition.y_ += incs.y_;	//increment y first
			moveDist.y_ -= fabs(incs.y_);

			map.objectToTileCoords(newPosition, &tx, &ty);	//next tile position (could be the same tile)
			if(map.isTileCollidable(tx, ty))				//crossed tile boundary in y direction
			{			
				newPosition.y_ = round(newPosition.y_, map.getTileHeight()) - sign(oldVelocity.y_)*TOLERANCE;
								
				earliestCollision = yTime;
				break;				
			}

            newPosition.x_ += incs.x_;	//increment x second
			moveDist.x_ -= fabs(incs.x_);

			map.objectToTileCoords(newPosition, &tx, &ty);	//next tile position (could be the same tile)
			if(map.isTileCollidable(tx, ty))
			{					
				newPosition.x_ = round(newPosition.x_, map.getTileWidth()) - sign(oldVelocity.x_)*TOLERANCE;
				
				earliestCollision = xTime;
				break;				
			}
		}
	}

	*collidePosition = newPosition;
	if(!earliestCollision)
        *collideTime = 0;
	else
        *collideTime = earliestCollision;

	return (earliestCollision != 0);
}

bool checkMapRectCollision(const TileMap& map, double timestep, const Rect& oldRect, const Vector& oldVelocity, Rect* collisionRect, double* collisionTime)
{
	double width = fabs(oldRect.right - oldRect.left);
	double height = fabs(oldRect.top - oldRect.bottom);

	Uint cols = (Uint)ceil(width / map.getTileWidth()) + 1;
	Uint rows = (Uint)ceil(height / map.getTileHeight()) + 1;
	Uint numPoints = rows*2 + cols*2 - 4;		//"surface area" of rectangle - ponints on the outside
	double cellWidth = width / (cols-1);
	double cellHeight = height / (rows-1);

	assert(cols > 1);
	assert(rows > 1);

	struct CollisionData
	{
		Vector point;
		Vector newPoint;
		Vector newVelocity;
		double collideTime;
	};
	CollisionData* points = new CollisionData[numPoints];		//"surface area" of rectangle

	Uint i, j, n;
	int minCollisionIndex;
	double earliestCollision = 0;

	Rect newRect = oldRect;

	while(timestep > 0)
	{
		for(i=0; i < 2*cols; ++i)	//fill in top and bottom row
		{
			points[i].point.x_ = newRect.left + (i % cols) * cellWidth;		
			points[i].point.y_ = newRect.top - (i / cols) * height;
			points[i].point.z_ = 0;
		}

		for(j=0; j < 2*(rows-2); ++j)	//fill in left and right column, without top and bottom point
		{
			points[i + j].point.x_ = newRect.left + (j%2) * width;	//left or right side
			points[i + j].point.y_ = newRect.top - ((j/2)+1) * cellHeight;
			points[i + j].point.z_ = 0;
		}

		for(n=0; n < numPoints; ++n)
		{
			checkMapCollision(map, timestep, points[n].point, oldVelocity, &points[n].newPoint, &points[n].collideTime);
		}

		minCollisionIndex = -1;
		for(n = 0; n < numPoints; ++n)		//get min time
		{
			if(points[n].collideTime > 0)
			{
				if(minCollisionIndex < 0 ||
					points[n].collideTime < points[minCollisionIndex].collideTime)
				{
					minCollisionIndex = n;
				}
			}
		}

		//update states after collision

		if(minCollisionIndex >= 0)		//collision response
		{
			//newVelocity = points[minCollisionIndex].newVelocity;
				
			if(minCollisionIndex < (int)cols*2)
			{		//e.g., col-2 needs to move left 2 cellWidths to get to the left
				newRect.left = points[minCollisionIndex].point.x_ - (minCollisionIndex % cols)*cellWidth;
				newRect.top = points[minCollisionIndex].point.y_ + (minCollisionIndex / cols)*height;
			}
			else
			{		//e.g., row-1 needs to move up 1 cellHeight to get to the top
				newRect.left = points[minCollisionIndex].point.x_ - (minCollisionIndex % 2)*width;
				newRect.top = points[minCollisionIndex].point.y_ + ((minCollisionIndex - cols*2)/2 + 1)*cellHeight;
			}

			newRect.right = newRect.left + width;
			newRect.bottom = newRect.top - height;

			if(!earliestCollision)
			{
				earliestCollision = points[minCollisionIndex].collideTime;
				break;
			}

			timestep -= points[minCollisionIndex].collideTime;
		}
		else	//no collisions
		{
			break;
		}
	}

	delete [] points;

	*collisionRect = newRect;
	*collisionTime = earliestCollision;
	
	return (earliestCollision != 0);
}
