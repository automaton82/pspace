#include "SubspaceBomb.h"

#include "TextureManager.h"

#include "SubspacePlayer.h"
#include "SubspaceSound.h"
#include "SubspaceZone.h"

SubspaceBomb::SubspaceBomb(int level, int shrapnel, int bouncing, bool emp, double proxRange, Uint shrapLevel, bool shrapBounce, bool isMine) : 
	level_(level),
	shrapnel_(shrapnel),
	bouncing_(bouncing),
	emp_(emp),
	shrapnelLevel_(shrapLevel),
	shrapnelBounce_(shrapBounce),
	isMine_(isMine),

	isProximityActive_(false)
{
	setProximityRange(proxRange);

	bombTexture_.load("graphics/bombs.bm2", false);
	bombTexture_.setFrameSize(16, 16);
	bombTexture_.setAnimationPeriod(500);

	mineTexture_.load("graphics/mines.bm2", false);
	mineTexture_.setFrameSize(16, 16);
	mineTexture_.setAnimationPeriod(500);

	setTexture();
}

SubspaceBomb::~SubspaceBomb()
{
	if(isMine_)	//its a mine
		owner_->mineDestroyed();
}

WeaponData SubspaceBomb::getWeaponSettings() const
{
	WeaponData w;
	w.u = 0;

	w.bouncing = shrapnelBounce_;			//for bouncing shrapnel bullets
	
	w.level = level_-1;
	w.shrapCount = shrapnel_;
	w.emp = emp_;
											//TODO: how to get yellow shrapnel?
	if(shrapnelLevel_ > 1)
		w.isBomb = true;						//is bullet a level 1 or 3
	else
		w.isBomb = false;

	w.alternate = false;
	
	if(proximityRange_ > 0)
		w.type = WEP_ProximityBomb;
	else
		w.type = WEP_Bomb;
			
	return w;
}

void SubspaceBomb::addTarget(SubspacePlayer* target)
{
	ProximityTarget p;
	p.distance = (this->getPosition() - target->getPosition()).magnitude();
	p.object = target;

	//if(p.distance < proximityRange_)
		targets_.push_back(p);
}

bool SubspaceBomb::isProximityActive() const
{
	return proximityRange_ > 0;
}

void SubspaceBomb::setProximityRange(double range)
{
	proximityRange_ = range;
	if(range > 0)
		isProximityActive_ = true;
	else
		isProximityActive_ = false;
}

double SubspaceBomb::getDamage(double distance) const
{
	double maxDmg = zone_->getArenaSettings().BombDamageLevel / 1000.0;
	if(bouncing_)
		maxDmg *= zone_->getArenaSettings().BBombDamagePercent / 1000.0;
	if(emp_)
		maxDmg *= zone_->getArenaSettings().EBombDamagePercent / 1000.0;

	double maxDist = zone_->getArenaSettings().BombExplodePixels * (getLevel()+1);

	double damage = 0;
	if(distance < maxDist)	//assume damage decreases linearly with distance
	{
		damage = (1.0 - (distance / maxDist)) * maxDmg;
	}

	return damage;
}

int SubspaceBomb::getLevel() const
{
	return level_;
}

Uint SubspaceBomb::getShrapnel() const
{
	return shrapnel_;
}

bool SubspaceBomb::getShrapnelBounce() const
{
	return shrapnelBounce_;
}

Uint SubspaceBomb::getShrapnelLevel() const
{
	return shrapnelLevel_;
}

void SubspaceBomb::setLevel(int l)
{
	level_ = l;
	
	setTexture();
}

void SubspaceBomb::setBouncing(int b)
{
	bouncing_ = b;

	setTexture();
}

void SubspaceBomb::setEmp(bool isEmp)
{
	emp_ = isEmp;
}

void SubspaceBomb::setTexture()
{
	bool mine = false;

	if(velocity_.magnitude() == 0)
		mine = true;

	if(!mine)	//is a bomb
	{
		bombTexture_.setFrameRange((level_-1)*10, (level_)*10);

		if(bouncing_ > 0)
			bombTexture_.setFrameRange((level_-1)*10 + 4*10*2, (level_)*10 + 4*10*2);
		else if(emp_)
			bombTexture_.setFrameRange((level_-1)*10 + 4*10, (level_)*10 + 4*10);
	}
	else	//is a mine
	{
		mineTexture_.setFrameRange((level_-1)*10, (level_)*10);

		if(emp_)
			mineTexture_.setFrameRange((level_-1)*10 + 4*10, (level_)*10 + 4*10);
	}

	if(level_ >= 5)	//thors hammer override
		bombTexture_.setFrameRange(4*10*3, 4*10*3 + 10);

}

bool SubspaceBomb::isBouncy() const
{
	bool retval = false;

	if(bouncing_ > 0)
		retval = true;
	
	return retval;
}

bool SubspaceBomb::isEmp() const
{
	return emp_;
}

bool SubspaceBomb::isMine() const
{
	return isMine_;
}

void SubspaceBomb::destroy()
{
	if(isEmp())
		subspaceSounds[SOUND_EBombEx].play(position_, velocity_ / 100.0);
	else
		subspaceSounds[SOUND_Explode2].play(position_, velocity_ / 100.0);

	zone_->destroyBomb(this);
}

void SubspaceBomb::draw() const
{
	if(velocity_.magnitude() == 0)	//mine
	{
		glTranslated(-(double)mineTexture_.getFrameWidth()/2, -(double)mineTexture_.getFrameHeight()/2, 0);
		mineTexture_.display();
	}
	else
	{
		glTranslated(-(double)bombTexture_.getFrameWidth()/2, -(double)bombTexture_.getFrameHeight()/2, 0);
		bombTexture_.display();
	}
}

void SubspaceBomb::update(double time)
{
	//TODO: update bouncing bombs
	//update
	//SubspaceWeapon::update(timeMultiplier);

	timeToLive_ -= time;

	if(timeToLive_ <= 0)
		this->garbage = true;

	Vector newPosition, newVelocity;

	//update for bouncing
	bool destroyOnCollide = !this->isBouncy();
	if(zone_ && zone_->handleMapCollision(time, getPosition(), getVelocity(), newPosition, newVelocity, 1.0, destroyOnCollide))
	{
		position_ = newPosition;
		velocity_ = newVelocity;

		if(!this->isBouncy())
			this->destroy();
		else
		{
			--bouncing_;
			if(bouncing_ == 0)
				setTexture();
		}
	}
	else
	{
	    SubspaceGameObject::update(time);		//velocity scaled here
	}

	if(velocity_.magnitude() == 0 && !isMine_)	//converted to mine
	{
		isMine_ = true;
		setTexture();
		owner_->mineFired();
	}

	if(velocity_.magnitude() != 0 && isMine_)	//oncverted to bomb
	{
		isMine_ = false;
		setTexture();
		owner_->mineDestroyed();
	}

	bombTexture_.advanceFrame(time);
	mineTexture_.advanceFrame(time);

	//update proximity targets
	TargetList::iterator i;
	for(i = targets_.begin(); i != targets_.end(); ++i)
	{
		ProximityTarget& target = (*i);

		double newDistance = (getPosition() - target.object->getPosition()).magnitude();

													//TODO: correct way to validate target? when player leaves

        if(target.distance < proximityRange_ &&		//in prox range
			newDistance > target.distance &&		//moving farther away
			!target.object->isDead())				//not dead
		{
			this->destroy();
		}
		else	//update distance
		{
			target.distance = newDistance;
		}
	}
}