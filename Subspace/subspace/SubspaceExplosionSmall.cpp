#include "SubspaceExplosionSmall.h"

SubspaceExplosionSmall::SubspaceExplosionSmall()
{
	double animationTime = 500;

	effectTexture_.load("graphics/explode0.bm2", false);
	effectTexture_.setFrameSize(16, 16);
	effectTexture_.setAnimationPeriod(animationTime);

	timeToLive_ = animationTime;
}

SubspaceExplosionSmall::~SubspaceExplosionSmall()
{
}