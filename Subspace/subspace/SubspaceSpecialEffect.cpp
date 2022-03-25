#include "SubspaceSpecialEffect.h"

SubspaceSpecialEffect::SubspaceSpecialEffect()
{
	//example
	/*double animationTime = 500;

	explosionTexture_.load("graphics/explosion0.bm2");
	explosionTexture_.setFrameSize(16, 16);
	explosionTexture_.setAnimationPeriod(animationTime);

	timeToLive_ = animationTime;*/
}

SubspaceSpecialEffect::~SubspaceSpecialEffect()
{
}

void SubspaceSpecialEffect::update(double timeStep)
{
	effectTexture_.advanceFrame(timeStep);

	timeToLive_ -= timeStep;
	if(timeToLive_ <= 0)
		this->setGarbage(true);

	SubspaceGameObject::update(timeStep);
}

void SubspaceSpecialEffect::draw() const
{
	glTranslated(-(int)effectTexture_.getFrameWidth()/2, -(int)effectTexture_.getFrameHeight()/2, 0);
	effectTexture_.display();
}
