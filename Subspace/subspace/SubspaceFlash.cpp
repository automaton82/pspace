#include "SubspaceFlash.h"

SubspaceFlash::SubspaceFlash()
{
	double animationTime = 500;

	effectTexture_.load("graphics/warp.bm2", false);
	effectTexture_.setFrameSize(48, 48);
	effectTexture_.setAnimationPeriod(animationTime);

	timeToLive_ = animationTime;
}

SubspaceFlash::~SubspaceFlash()
{
}