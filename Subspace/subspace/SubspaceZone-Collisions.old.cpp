#include "SubspaceZone.h"

#include <float.h>		//for FLT_MAX

#include "MathUtil.h"
using namespace MathUtil;

#include "SubspaceUtil.h"

#include "SubspaceBomb.h"
#include "SubspaceBullet.h"
#include "SubspaceFlag.h"
#include "SubspacePlayer.h"


double SubspaceZone::handleMapCollision(double time, const Vector& oldPosition, const Vector& oldVelocity, Vector& newPosition, Vector& newVelocity, double friction, bool stopAfterCollision)
{
	return handleMapCollisionsHelper(time / 10000.0, oldPosition, oldVelocity, newPosition, newVelocity, friction, stopAfterCollision);
}

/////////////////////////////////////
//based on Bresenham's line drawing algorithm - discrete steps in either x or y direction
/////////////////////////////////////
double SubspaceZone::handleMapCollisionsHelper(double time, const Vector& oldPosition, const Vector& oldVelocity, Vector& newPosition, Vector& newVelocity, double friction, bool stopAfterCollision)
{
	const SubspaceMap* map = this->getMap();

	if(!map)
		return false;

	Vector finalPos = oldPosition + oldVelocity*time;	//next position assuming no collision
	Vector finalVel = oldVelocity;						//next velocity assuming no collision
	Vector moveDist = abs(finalPos - oldPosition);			//total movement
	
	Vector incs;
	calcDistanceStep(time, oldVelocity, incs);	
	//double incRatio = incs.y_ / incs.x_;
			
	double xDist, xTime;
	double yDist, yTime;

	Uint tx, ty;
	double earliestCollision = 0;		//TODO: 0 retval means no collision - could that cause problems?

	newVelocity = oldVelocity;
	newPosition = oldPosition;

	int ct=0;

    while(moveDist.x_ + moveDist.y_ > TOLERANCE)
	{														//check every increment (largest is a whole tile)
		xDist = min(fabs(moveDist.x_), SubspaceMap::tileWidth - TOLERANCE);		//move maximum of 1 tile
		incs.x_ = xDist * sign(newVelocity.x_);
			
		if(newVelocity.x_ == 0)
			xTime = DBL_MAX;
		else
			xTime = fabs(xDist / newVelocity.x_);
				
		yDist = min(fabs(moveDist.y_), SubspaceMap::tileHeight - TOLERANCE);
		incs.y_ = yDist * sign(newVelocity.y_);
		
		if(newVelocity.y_ == 0)
			yTime = DBL_MAX;
		else
			yTime = fabs(yDist / newVelocity.y_);
		
		//assert(fabs(incs.x_) < 16);
		//assert(fabs(incs.y_) < 16);
		
		Vector temp, tempVel;

		if(xTime <= yTime)
		{   
			newPosition.x_ += incs.x_;	//increment x first
			moveDist.x_ -= fabs(incs.x_);

			objectToTileCoords(newPosition, &tx, &ty);	//next tile position (could be the same tile)
			if(map->getTile(tx, ty).isCollidable())
			{
				temp = newPosition;
				tempVel = newVelocity;
				//handleMapCollisionX(friction, newPosition, newVelocity, newPosition, newVelocity);
				handleMapCollisionX(newPosition.x_, newVelocity.x_, &newPosition.x_, &newVelocity.x_);

				//add extra distance back on
				moveDist.x_ += newPosition.x_ - temp.x_;

				//add friction
				newVelocity *= friction;
			
				if(!earliestCollision)
					earliestCollision = xTime;				
			}

			if(stopAfterCollision && earliestCollision)
				break;

			newPosition.y_ += incs.y_;
			moveDist.y_ -= fabs(incs.y_);

			objectToTileCoords(newPosition, &tx, &ty);	//next tile position (could be the same tile)
			if(map->getTile(tx, ty).isCollidable())		//crossed tile boundary in y direction
			{	
				temp = newPosition;
				tempVel = newVelocity;
				//handleMapCollisionY(friction, newPosition, newVelocity, newPosition, newVelocity);
				handleMapCollisionY(newPosition.y_, newVelocity.y_, &newPosition.y_, &newVelocity.y_);
				//add extra distance back on
				moveDist.y_ += newPosition.y_ - temp.y_;

				//add friction
				newVelocity *= friction;

				if(!earliestCollision)
					earliestCollision = yTime;

				/*if(newPosition.x_ > 8272-TOLERANCE)
				{
					printf("DESTROY ERROR\n");
				}*/
			}

			if(stopAfterCollision && earliestCollision)
				break;
		}
		else
		{
			newPosition.y_ += incs.y_;
			moveDist.y_ -= fabs(incs.y_);

			objectToTileCoords(newPosition, &tx, &ty);	//next tile position (could be the same tile)
			if(map->getTile(tx, ty).isCollidable())		//crossed tile boundary in y direction
			{			
				temp = newPosition;
				tempVel = newVelocity;
				handleMapCollisionY(newPosition.y_, newVelocity.y_, &newPosition.y_, &newVelocity.y_);
				//add extra distance back on
				moveDist.y_ += newPosition.y_ - temp.y_;

				//add friction
				newVelocity *= friction;

				if(!earliestCollision)
					earliestCollision = yTime;
			}
			
			if(stopAfterCollision && earliestCollision)
				break;

            newPosition.x_ += incs.x_;
			moveDist.x_ -= fabs(incs.x_);

			objectToTileCoords(newPosition, &tx, &ty);	//next tile position (could be the same tile)
			if(map->getTile(tx, ty).isCollidable())
			{					
				temp = newPosition;
				tempVel = newVelocity;
				handleMapCollisionX(newPosition.x_, newVelocity.x_, &newPosition.x_, &newVelocity.x_);
				//add extra distance back on
				moveDist.x_ += newPosition.x_ - temp.x_;

				//add friction
				newVelocity *= friction;
				
				if(!earliestCollision)
					earliestCollision = xTime;
			}

			if(stopAfterCollision && earliestCollision)
				break;
		}
	}

	return earliestCollision;
}


double SubspaceZone::handleMapBoxCollision(double time, const Rect& oldRect, const Vector& oldVelocity, Rect& newRect, Vector& newVelocity, double friction, bool stopAfterCollision)
{
	//TODO: necessary?
	return handleMapRectCollision(time, oldRect, oldVelocity, newRect, newVelocity, friction, stopAfterCollision);




	/*const int numPoints = 4;	//top left, top right, bottom left, bottom right
	int i, minCollisionIndex;
	bool collided = false;

	Vector points[numPoints];		
	Vector newPoints[numPoints];
	double collideTimes[numPoints];
	Vector newVels[numPoints];

	newVelocity = oldVelocity;
	newRect = oldRect;

	double width = fabs(oldRect.right - oldRect.left);
	double height = fabs(oldRect.top - oldRect.bottom);

	while(time > 0)
	{
		points[0] = Vector(newRect.left, newRect.top, 0);												
		points[1] = Vector(newRect.right, newRect.top, 0);	
		points[2] = Vector(newRect.left, newRect.bottom, 0);
		points[3] = Vector(newRect.right, newRect.bottom, 0);

		collideTimes[0] = handleMapCollision(time, points[0], newVelocity, newPoints[0], newVels[0], friction, true);
		collideTimes[1] = handleMapCollision(time, points[1], newVelocity, newPoints[1], newVels[1], friction, true);
		collideTimes[2] = handleMapCollision(time, points[2], newVelocity, newPoints[2], newVels[2], friction, true);
		collideTimes[3] = handleMapCollision(time, points[3], newVelocity, newPoints[3], newVels[3], friction, true);

		minCollisionIndex = -1;
		for(i = 0; i < numPoints; ++i)		//get min time
		{
			if(collideTimes[i] > 0)
			{
				if(minCollisionIndex < 0 ||
					collideTimes[i] < collideTimes[minCollisionIndex])
				{
					minCollisionIndex = i;
				}
			}
		}

		//update states after collision

		if(minCollisionIndex >= 0)		//collision response
		{
			newVelocity = newVels[minCollisionIndex];

			if(minCollisionIndex % 2 == 0)	//0 or 2, top left, bottom left
			{	
				newRect.left = newPoints[minCollisionIndex].x_;		
				newRect.right = newRect.left + width;
			}
			else
			{
				newRect.right = newPoints[minCollisionIndex].x_;
				newRect.left = newRect.right - width;
			}

			if(minCollisionIndex < 2)	//0 or 1, top left, top right
			{
				newRect.top = newPoints[minCollisionIndex].y_;		
				newRect.bottom = newRect.top - height;
			}
			else
			{
				newRect.bottom = newPoints[minCollisionIndex].y_;
				newRect.top = newRect.bottom + height;
			}

			time -= collideTimes[minCollisionIndex];
			collided = true;
		}
		else	//no collisions
		{
			break;
		}
	}
	
	return collided;*/
}

double SubspaceZone::handleMapRectCollision(double time, const Rect& oldRect, const Vector& oldVelocity, Rect& newRect, Vector& newVelocity, double friction, bool stopAfterCollision)
{
	double width = fabs(oldRect.right - oldRect.left);
	double height = fabs(oldRect.top - oldRect.bottom);

	Uint cols = (Uint)ceil(width / SubspaceMap::tileWidth) + 1;
	Uint rows = (Uint)ceil(height / SubspaceMap::tileHeight) + 1;
	Uint numPoints = rows*2 + cols*2 - 4;		//"surface area" of rectangle
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

	newVelocity = oldVelocity;
	newRect = oldRect;

	while(time > 0)
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
			points[n].collideTime = handleMapCollision(time, points[n].point, newVelocity, points[n].newPoint, points[n].newVelocity, friction, true);
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
			newVelocity = points[minCollisionIndex].newVelocity;
				
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
				earliestCollision = points[minCollisionIndex].collideTime;

			time -= points[minCollisionIndex].collideTime;
		}
		else	//no collisions
		{
			break;
		}
	}

	delete [] points;
	
	return earliestCollision;
}

void SubspaceZone::handleMapCollisionX(double oldPosition, double oldVelocity, double* newPosition, double* newVelocity)
{
	//move to edge of tile
	if(oldVelocity < 0)
		*newPosition = ceil(oldPosition, SubspaceMap::tileWidth) - sign(oldVelocity)*TOLERANCE;		//moving left, move to right edge of tile
	else
		*newPosition = floor(oldPosition, SubspaceMap::tileWidth) - sign(oldVelocity)*TOLERANCE;	//moving right, move to left edge of tile

	*newVelocity = -1 * oldVelocity;
}

void SubspaceZone::handleMapCollisionY(double oldPosition, double oldVelocity, double* newPosition, double* newVelocity)
{
	//move to edge of tile
	if(oldVelocity < 0)
		*newPosition = ceil(oldPosition, SubspaceMap::tileHeight) - sign(oldVelocity)*TOLERANCE;	//moving down, move to top edge of tile
	else
		*newPosition = floor(oldPosition, SubspaceMap::tileHeight) - sign(oldVelocity)*TOLERANCE;	//moving up, move to bottom edge of tile

	//*newPosition += (*newPosition-oldPosition);

	*newVelocity = -1 * oldVelocity;
}


bool SubspaceZone::handleMapCollisionX(double friction, const Vector& nextPos, const Vector& vel, Vector& newPosition, Vector& newVelocity)
{
	/*if(vel.x_ < 0)
		newPosition.x_ = ceil(nextPos.x_, SubspaceMap::tileWidth) - sign(vel.x_)*TOLERANCE;
	else
		newPosition.x_ = floor(nextPos.x_, SubspaceMap::tileWidth) - sign(vel.x_)*TOLERANCE;*/
	newPosition.x_ = round(nextPos.x_, SubspaceMap::tileWidth) - sign(vel.x_)*TOLERANCE;
	
	//newPosition.x_ = (int)((nextPos.x_+w/2.0)/w)*w - xDir*tolerance;	//move to tile boundary
	newVelocity.x_ *= -1;
	newVelocity *= friction;

	return true;
}

bool SubspaceZone::handleMapCollisionY(double friction, const Vector& nextPos, const Vector& vel, Vector& newPosition, Vector& newVelocity)
{
	/*if(vel.y_ < 0)
        newPosition.y_ = ceil(nextPos.y_, SubspaceMap::tileHeight) - sign(vel.y_)*TOLERANCE;
	else
		newPosition.y_ = floor(nextPos.y_, SubspaceMap::tileHeight) - sign(vel.y_)*TOLERANCE;*/

	newPosition.y_ = round(nextPos.y_, SubspaceMap::tileHeight) - sign(vel.y_)*TOLERANCE;
	//newPosition.y_ = (int)((nextPos.y_+h/2.0)/h)*h - yDir*tolerance;	//move to tile boundary
	newVelocity.y_ *= -1;
	newVelocity *= friction;

	return true;
}

void SubspaceZone::calcDistanceStep(double time, const Vector& vel, Vector& increments)
{
	//ignore z component in subspace
	if(fabs(vel.x_) > fabs(vel.y_))	
	{						//larger increment is in x
		increments.x_ = min(SubspaceMap::tileWidth - TOLERANCE, fabs(vel.x_*time)) * sign(vel.x_);
		//increments.x_ = vel.x_*time;
		increments.y_ = vel.y_/vel.x_ * increments.x_;
	}
	else
	{						//larger increment is in y
		increments.y_ = min(SubspaceMap::tileHeight - TOLERANCE, fabs(vel.y_*time)) * sign(vel.y_);
		//increments.y_ = vel.y_*time;
		increments.x_ = vel.x_/vel.y_ * increments.y_;
	}
}

double SubspaceZone::calcDistanceToTile(double pos, double vel, double tileSize)
{
	double dist;

	if(vel >= 0)		//moving right
	{
		dist = tileSize - fmod(pos, tileSize);
		/*if(dist == 0)
			dist = tileSize - dist;*/
	}
	else				//moving left
	{
		dist = fmod(pos, tileSize);		//distance to nearest tile in x direction, in direction of travel
		/*if(dist == 0)
			dist = tileSize - dist;*/
	}

	/*if(dist == 0)
		dist = tileSize - dist;*/
		
	//dist = min(fabs(fmod(pos, tileSize)), tileSize - fabs(fmod(pos, tileSize)) );

	return dist;
}

double SubspaceZone::calcTimeToTile(double pos, double vel, double tileSize)
{
	//time until an x-collision
	double dist;
	dist = calcDistanceToTile(pos, vel, tileSize);

	return fabs(vel / dist);
}

void SubspaceZone::handleMapObjectCollisions()
{
	SubspacePlayerMap::iterator i;
	for(i = players_.begin(); i != players_.end(); ++i)
	{
		SubspacePlayer* p = (*i).second;
		assert(p);

		if(p->getShipType() != SHIP_Spectator)
		{
			//check player box
			ObjectList objects;

			Rect playerBox(p->getPosition().y_ + p->getSize()/2,
				p->getPosition().x_ - p->getSize()/2,
				p->getPosition().y_ - p->getSize()/2,
				p->getPosition().x_ + p->getSize()/2);
				
			getTileObjectsInBox(playerBox, objects);

			SubspaceMap::ObjectList::iterator j;
			for(j = objects.begin(); j != objects.end(); ++j)
			{
				SubspaceGameObject* g = (*j);

				Uint tileX = (Uint) (g->getPosition().x_ / SubspaceMap::tileWidth);
				Uint tileY = (Uint) (g->getPosition().y_ / SubspaceMap::tileHeight);

				//assert(map_.getTileObject(tileX, tileY) == g);

				SubspacePrize* prize;
				SubspaceFlag* flag;

				if(flag = dynamic_cast<SubspaceFlag*>(g))
				{
					if(flag->getTeam() != p->getTeam())
					{
						//playerGotFlag(p->getId(), flag->getFlagID());
						playerRequestFlag(p->getId(), flag->getFlagID());
					}
					else
						//re-add flag
						map_.addObject(tileX, tileY, flag, false);
				}
				else if(prize = dynamic_cast<SubspacePrize*>(g))
				{
					playerGotPrize(p->getId(), prize->getPrizeType(), tileX, tileY);
					--numPrizes_;
					displayManager_[MapItemLayer].removeObject(prize);
					delete prize;
				}
				else
				{
					debugout << "Unknown object type" << endl;
				}
			}
		}
	}
}

void SubspaceZone::getTileObjectsInBox(const Rect& rect, ObjectList& objects)
{
	assert(rect.left <= rect.right && rect.bottom <= rect.top);

	/*Uint startX = rect.left / SubspaceMap::tileWidth;
	Uint startY = (rect.bottom) / SubspaceMap::tileHeight;

	Uint lastX = (rect.right+0.5) / SubspaceMap::tileWidth;
	Uint lastY = (rect.top+0.5) / SubspaceMap::tileHeight;*/

	double startXf = rect.left / SubspaceMap::tileWidth;
	double startYf = rect.bottom / SubspaceMap::tileWidth;

	double lastXf = rect.right / SubspaceMap::tileWidth;
	double lastYf = rect.top / SubspaceMap::tileWidth;

	double epsilon = 0.1;

	startXf = MathUtil::clamp(startXf, epsilon);
	lastXf = MathUtil::clamp(lastXf, epsilon);

	startYf = MathUtil::clamp(startYf, epsilon);
	lastYf = MathUtil::clamp(lastYf, epsilon);

	Uint startX = (Uint)startXf;
    Uint lastX = (Uint)ceil(lastXf);

	Uint startY = (Uint)startYf;
    Uint lastY = (Uint)ceil(lastYf);

	for(Uint i=startX; i < lastX; ++i)
	{
		for(Uint j=startY; j < lastY; ++j)
		{
			SubspaceGameObject* g = map_.getTileObject(i, j);
			if(g)
			{
				objects.push_back(g);
			}

			//map_.testTiles_.push_back(SubspaceTile(i, j, 1));
		}
	}
}

void SubspaceZone::handlePlayerObjectCollisions()
{
	//NOTE: all weapons treated as points
	SubspacePlayerMap::iterator i;
	for(i = players_.begin(); i != players_.end(); ++i)
	{
		SubspacePlayer* player = (*i).second;

		assert(player);

		if(!player->isSpectator())
		{
			//bullets 
			handleBulletCollisions(player);

			//bombs
			handleBombCollisions(player);
		}

		//specials
	}
}

void SubspaceZone::handleBulletCollisions(SubspacePlayer* player)
{
	if(player->getShipType() == SHIP_Spectator || player->isDead())
		return;

	Rect box = player->getBoundingBox();

	BulletList::iterator bul;
	for(bul = bullets_.begin(); bul != bullets_.end(); ++bul)
	{
		SubspaceBullet* bullet = (*bul);
		if(bullet->getOwner()->getTeam() != player->getTeam())	//different teams
		{
			if(pointInBox(bullet->getPosition().x_, bullet->getPosition().y_, box))	//collision occured
			{
				double damage = bullet->getDamage();

				player->applyDamage(damage);

				displayManager_[WeaponLayer].removeObject(bullet);
				bullet->destroy();				
				//destroyBullet(bullet);

				/*delete bullet;
				bullet = 0;
                bul = bullets_.erase(bul);
				--bul;*/
			}
		}
	}
}

void SubspaceZone::handleBombCollisions(SubspacePlayer* player)
{
	if(player->getShipType() == SHIP_Spectator || player->isDead())
		return;

	Rect box = player->getBoundingBox();

	BombList::iterator b;
	for(b = bombs_.begin(); b != bombs_.end(); ++b)
	{
		SubspaceBomb* bomb = (*b);
		if(bomb->getOwner()->getTeam() != player->getTeam())	//different teams
		{
			if(pointInBox(bomb->getPosition().x_, bomb->getPosition().y_, box))	//collision occured
			{
				double damage = bomb->getDamage();

				player->applyDamage(damage);

				displayManager_[WeaponLayer].removeObject(bomb);
				destroyBomb(bomb);

				/*delete bomb;
				bomb = 0;
                b = bombs_.erase(b);
				--b;*/
			}
		}
	}
}