#include "SubspaceExplosion.h"

#include "Color.h"

SubspaceExplosion::SubspaceExplosion()
{
	double animationTime = 1000;

	effectTexture_.load("graphics/explode2.bm2", false);
	effectTexture_.setFrameSize(80, 80);
	effectTexture_.setAnimationPeriod(animationTime);

	timeToLive_ = animationTime;
}

SubspaceExplosion::~SubspaceExplosion()
{
}

void SubspaceExplosion::draw() const
{
	Color c(1, 1, 1, 1);
	c.enable();

	SubspaceSpecialEffect::draw();
}