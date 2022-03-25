#include "SubspaceRepel.h"

#include "SubspaceZone.h"

#include "MathUtil.h"
using namespace MathUtil;

SubspaceRepel::SubspaceRepel()
{
	double animationTime = 500;

	repelTexture_.load("graphics/repel.bm2", false);
	repelTexture_.setFrameSize(96, 96);
	repelTexture_.setAnimationPeriod(animationTime);

	doDisplay_ = true;
}

SubspaceRepel::~SubspaceRepel()
{
}

WeaponData SubspaceRepel::getWeaponSettings() const
{
	WeaponData w;
	w.u = 0;
	w.type = WEP_Repel;
	
	return w;
}

void SubspaceRepel::addTarget(SubspaceGameObject* target)
{
	targetData newTarget;
	newTarget.dir = target->getPosition() - this->getPosition();
	newTarget.object = target;

	targets_.push_back(newTarget);
}

// TODO: this needs to be redesigned in case target no longer exists
void SubspaceRepel::updateTargets()
{
	/*TargetList::iterator i;
	for(i = targets_.begin(); i != targets_.end(); ++i)
	{
		targetData& target = (*i);
	
		Vector vel = target.dir.unitVector() * zone_->getArenaSettings().RepelSpeed;
		target.object->setVelocity(vel);
	}

	targets_.clear();	//targets must be refreshed every cycle*/
}

void SubspaceRepel::update(double timeMultiplier)
{
	Uint frame=0, nextFrame=0;

	frame = repelTexture_.getFrame();
	repelTexture_.advanceFrame(timeMultiplier);
	nextFrame = repelTexture_.getFrame();

	if(nextFrame < frame)
		doDisplay_ = false;

	SubspaceWeapon::update(timeMultiplier);
	updateTargets();
}

void SubspaceRepel::draw() const
{
	if(doDisplay_)
	{
		glTranslated(-(int)repelTexture_.getFrameWidth()/2, -(int)repelTexture_.getFrameHeight()/2, 0);
		repelTexture_.display();
	}
}