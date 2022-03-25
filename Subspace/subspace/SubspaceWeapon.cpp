#include "SubspaceWeapon.h"

#include "SubspaceZone.h"

SubspaceWeapon::SubspaceWeapon(SubspacePlayer* owner)
: owner_(owner)
{
}

SubspaceWeapon::~SubspaceWeapon()
{
}

SubspacePlayer* SubspaceWeapon::getOwner() const
{
	return owner_;
}

void SubspaceWeapon::setOwner(SubspacePlayer* owner)
{
	owner_ = owner;
}

void SubspaceWeapon::setZone(SubspaceZone* zone)
{
	zone_ = zone;
}

SubspaceZone* SubspaceWeapon::getZone() const
{
	return zone_;
}

WeaponType SubspaceWeapon::getWeaponType() const
{
	return weaponType;
}

WeaponData SubspaceWeapon::getWeaponSettings() const
{
	WeaponData w;
	w.u = 0;

	return w;
}

bool SubspaceWeapon::isBouncy() const
{
	return false;
}

void SubspaceWeapon::destroy()
{
	//should be implemented by sub-classes
}

void SubspaceWeapon::setTimeToLive(double timeToLive)
{
	timeToLive_ = timeToLive;
}

double SubspaceWeapon::getTimeToLive() const
{
	return timeToLive_;
}

void SubspaceWeapon::update(double time)
{
	timeToLive_ -= time;

	if(timeToLive_ <= 0)
		this->garbage = true;

	Vector newPosition, newVelocity;

	//lastPosition = position_;

	bool destroyOnCollide = !this->isBouncy();
	if(zone_ && zone_->handleMapCollision(time, getPosition(), getVelocity(), newPosition, newVelocity, 1.0, destroyOnCollide))
	{
		position_ = newPosition;
		velocity_ = newVelocity;

		if(!this->isBouncy())
			this->destroy();
		/*if(this->isBouncy())
		{
			position_ = newPosition;
			velocity_ = newVelocity;
		}
		else
		{
			this->destroy();
		}*/
	}
	else
	{
	    SubspaceGameObject::update(time);		//velocity scaled here
	}
	//SubspaceGameObject::update(time);
}