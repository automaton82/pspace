#include "SubspacePortal.h"

#include "SubspacePlayer.h"

SubspacePortal::SubspacePortal()
{
	double animationTime = 500;

	portalTexture_.load("graphics/warppnt.bm2", false);
	portalTexture_.setFrameSize(16, 16);
	portalTexture_.setAnimationPeriod(animationTime);
}

SubspacePortal::~SubspacePortal()
{
	this->owner_->clearPortal();
}

void SubspacePortal::update(double timeMultiplier)
{
	portalTexture_.advanceFrame(timeMultiplier);
	
	SubspaceWeapon::update(timeMultiplier);
}

void SubspacePortal::draw() const
{
	glTranslated(-(int)portalTexture_.getFrameWidth()/2, -(int)portalTexture_.getFrameHeight()/2, 0);
	portalTexture_.display();
}