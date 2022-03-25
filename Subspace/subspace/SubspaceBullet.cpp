#include "SubspaceBullet.h"

#include "TextureAnimated.h"
#include "TextureManager.h"
#include "SubspaceSound.h"
#include "SubspaceZone.h"

#include "MathUtil.h"
using namespace MathUtil;

#include "Material.h"

SubspaceBullet::SubspaceBullet(int l, bool b) : SubspaceWeapon(),
	level_(l), bouncing_(b)
{
	bulletTexture_.load("graphics/bullets.bm2", false);
	bulletTexture_.setFrameSize(5, 5);
	bulletTexture_.setAnimationPeriod(200);

	setLevel(l);
	setBouncing(b);
}

SubspaceBullet::~SubspaceBullet()
{
}


WeaponData SubspaceBullet::getWeaponSettings() const
{
	WeaponData w;
	w.u = 0;
	w.bouncing = bouncing_;
	w.level = level_-1;
	//w.level = 1;
	w.alternate = 0;

	if(w.bouncing)
		w.type = WEP_BouncingBullet;
	else
		w.type = WEP_Bullet;
	
	return w;
}

void SubspaceBullet::update(double time)
{
	SubspaceWeapon::update(time);

	bulletTexture_.advanceFrame(time);
}

int SubspaceBullet::getLevel() const
{
	return level_;
}

double SubspaceBullet::getDamage() const
{
	Uint32 maxDmg = (Uint32)(zone_->getArenaSettings().BulletDamageLevel/1000.0 + (getLevel()-1)*zone_->getArenaSettings().BulletDamageUpgrade/1000.0);
	//Uint32 maxDmg = bulletDmg + (getLevel()-1)*zone_->getArenaSettings().BulletDamageUpgrade/1000.0;
	//if(getLevel() == 4)	//burst bullet

	double retval = sqrt(unitRand() * (maxDmg*maxDmg + 1));		//formula for bullet damage

	return retval;
}

void SubspaceBullet::setLevel(int l)
{
	level_ = l;
	
	setTexture();
}

void SubspaceBullet::setBouncing(bool b)
{
	bouncing_ = b;

	setTexture();
}

void SubspaceBullet::setTexture()
{
	if(level_ < 1)
		return;

	if(bouncing_)
        bulletTexture_.setFrameRange((level_-1)*4 + 20, level_*4 + 20);
	else
        bulletTexture_.setFrameRange((level_-1)*4, level_*4);
}

bool SubspaceBullet::isBouncy() const
{
	return bouncing_;
}

void SubspaceBullet::destroy()
{
	subspaceSounds[SOUND_Explode0].play();

	zone_->destroyBullet(this);
}

void SubspaceBullet::draw() const
{
	glTranslated(-(int)bulletTexture_.getFrameWidth()/2, -(int)bulletTexture_.getFrameHeight()/2, 0);
	bulletTexture_.display();
}