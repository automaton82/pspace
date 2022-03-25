#include "SubspaceShrapnel.h"

#include "SubspaceSound.h"
#include "SubspaceZone.h"

#include "MathUtil.h"
using namespace MathUtil;

const double SubspaceShrapnel::inactiveTime = 250.0;

SubspaceShrapnel::SubspaceShrapnel(int level, bool bouncing) : SubspaceBullet(level, bouncing),
	timeAlive_(0)
{
	
	bulletTexture_.load("graphics/shrapnel.bm2", false);
	bulletTexture_.setFrameSize(8, 8);
	bulletTexture_.setAnimationPeriod(250);

	setTexture();
}

SubspaceShrapnel::~SubspaceShrapnel()
{
}

double SubspaceShrapnel::getDamage() const
{
	double damage = 0;

	if(zone_)
	{
		if(isActive())	
			damage = zone_->getArenaSettings().ShrapnelDamagePercent/10.0 * SubspaceBullet::getDamage();	//scales damage of normal bullet of same level
		else
			damage = zone_->getArenaSettings().InactiveShrapnelDamage;
	}

	return damage;
}

void SubspaceShrapnel::setTexture()
{
	if(level_ < 1)
		return;

	if(bouncing_)
		bulletTexture_.setFrameRange((level_-1)*10 + 30, level_*10 + 30);        
	else
		bulletTexture_.setFrameRange((level_-1)*10, level_*10);        
}

bool SubspaceShrapnel::isActive() const
{
	return (timeAlive_ >= inactiveTime);
}

bool SubspaceShrapnel::isBouncy() const
{
	if(isActive())
		return bouncing_;
	else
		return false;
}

void SubspaceShrapnel::destroy()
{
	if(isActive())
		subspaceSounds[SOUND_Explode0].play();

	zone_->destroyBullet(this);
}

void SubspaceShrapnel::update(double time)
{
	timeAlive_ += time;

	SubspaceBullet::update(time);
}
