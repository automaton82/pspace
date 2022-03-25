#include "SubspaceShipExplosion.h"

#include "Color.h"

SubspaceShipExplosion::SubspaceShipExplosion()
{
	double animationTime = 1000;

	effectTexture_.load("graphics/explode1.bm2", false);
	effectTexture_.setFrameSize(48, 48);
	effectTexture_.setAnimationPeriod(animationTime);

	timeToLive_ = animationTime;
}

SubspaceShipExplosion::~SubspaceShipExplosion()
{
}

void SubspaceShipExplosion::draw() const
{
	Color c(1, 1, 1, 1);
	c.enable();

	SubspaceSpecialEffect::draw();
}