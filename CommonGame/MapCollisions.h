#ifndef _MAPCOLLISIONS_H_
#define _MAPCOLLISIONS_H_

#include "Rect.h"
#include "TileMap.h"
#include "Vector.h"

const double TOLERANCE = 0.001;
	
// collidePosition is either the collision point, or the position after timestep, collisionTime is either the time at collision or 0
bool checkMapCollision(const TileMap& map, double timestep, const Vector& oldPosition, const Vector& oldVelocity, Vector* collidePosition, double* collideTime);
bool checkMapRectCollision(const TileMap& map, double timestep, const Rect& oldRect, const Vector& oldVelocity, Rect* newRect, double* collideTime);

#endif