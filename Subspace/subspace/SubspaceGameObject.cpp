#include "SubspaceGameObject.h"

#include "SubspaceMap.h"

SubspaceGameObject::SubspaceGameObject()
{
}

SubspaceGameObject::~SubspaceGameObject()
{
}

void SubspaceGameObject::setSubspaceDirection(int dir)
{
	double angle = (-dir * 9 + 90) * DEGREES_TO_RADIANS;

	Sprite::setDirection( Vector( cos(angle), sin(angle), 0 ) );
}

int SubspaceGameObject::getSubspaceDirection() const
{
	double angle = atan2(direction_.y_, direction_.x_) * RADIANS_TO_DEGREES;

	return static_cast<int>((-angle + 90 + 360) / 9);
}

void SubspaceGameObject::setDirection(const Vector& direction)
{
	Sprite::setDirection(direction);
}

void SubspaceGameObject::setSubspacePosition(const Vector& position)
{
	static int maxPosition = SubspaceMap::maxTileY * SubspaceMap::tileHeight;	
	Sprite::setPosition(Vector(position.x_, maxPosition - position.y_, position.z_));
}

const Vector& SubspaceGameObject::getSubspacePosition() const
{
	static int maxPosition = SubspaceMap::maxTileY * SubspaceMap::tileHeight;
	static Vector subspacePos;

	subspacePos = this->getPosition();
	subspacePos.y_ = maxPosition - subspacePos.y_;
	
	return subspacePos;
}

const Vector& SubspaceGameObject::getNextSubspacePosition(double time) const
{
	static Vector nextPos = getSubspacePosition() + getSubspaceVelocity()*time;

	return nextPos;
}

void SubspaceGameObject::setSubspaceVelocity(const Vector& velocity)
{
	Sprite::setVelocity(Vector(velocity.x_, velocity.y_ * -1, velocity.z_));
}

const Vector& SubspaceGameObject::getSubspaceVelocity() const
{
	static Vector subspaceVel;

	subspaceVel = this->getVelocity();
	subspaceVel.y_ *= -1;
	
	return subspaceVel;
}

void SubspaceGameObject::update(double time)
{
	GameObject::update(time / 10000.0);
}

void SubspaceGameObject::subspacePositionToStandardPosition(Vector& pos)
{
	Uint maxYPosition = (SubspaceMap::maxTileY-1) * SubspaceMap::tileHeight;
	pos.y_ = maxYPosition - pos.y_;
}

void SubspaceGameObject::standardPositionToSubspacePosition(Vector& pos)
{
	Uint maxYPosition = (SubspaceMap::maxTileY-1) * SubspaceMap::tileHeight;
	pos.y_ = maxYPosition - pos.y_;
}

void SubspaceGameObject::subspaceVelocityToStandardVelocity(Vector& vel)	//changes position
{
	vel.y_ *= -1;
}

Uint SubspaceGameObject::getTileX() const
{
	Uint x = (Uint)position_.x_ / SubspaceMap::tileWidth;
	return x;
}

Uint SubspaceGameObject::getTileY() const
{
	Uint y = (Uint)position_.y_ / SubspaceMap::tileHeight;
	return y;
}	

Uint SubspaceGameObject::getSubspaceTileX() const
{
	return getTileX();
}

Uint SubspaceGameObject::getSubspaceTileY() const
{
	Uint y = SubspaceMap::maxTileY - getTileY() - 1;

	return y;
}