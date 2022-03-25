#include "SubspaceBurstBullet.h"

#include "SubspaceZone.h"

#include "MathUtil.h"
using namespace MathUtil;

SubspaceBurstBullet::SubspaceBurstBullet() : SubspaceBullet(5, false), isActive_(false)
{
}

SubspaceBurstBullet::~SubspaceBurstBullet()
{
}

double SubspaceBurstBullet::getDamage() const
{
	Uint32 maxDmg = (Uint32) (zone_->getArenaSettings().BurstDamageLevel / 1000.0);
	double retval = sqrt(unitRand() * (maxDmg*maxDmg + 1));

	return retval;
}

WeaponData SubspaceBurstBullet::getWeaponSettings() const
{
	WeaponData w;
	w.u = 0;
	w.bouncing = 1;
	w.emp = 0;
	w.shrapCount = 0;
	w.isBomb = 0;
	w.level = 0;
	w.type = WEP_Burst;
	
	return w;
}

bool SubspaceBurstBullet::isActive() const
{
	return isActive_;
}

void SubspaceBurstBullet::update(double time)
{
	timeToLive_ -= time;

	if(timeToLive_ <= 0)
		this->garbage = true;

	Vector newPosition, newVelocity;

	if(zone_ && zone_->handleMapCollision(time, getPosition(), getVelocity(), newPosition, newVelocity))
	{
		isActive_ = true;
		bouncing_ = true;	//changes animation

		setTexture();

		position_ = newPosition;
		velocity_ = newVelocity;

		if(!this->isBouncy())
			this->destroy();
	}
	else
	{
		SubspaceBullet::update(time);
	}

	bulletTexture_.advanceFrame(time);
}