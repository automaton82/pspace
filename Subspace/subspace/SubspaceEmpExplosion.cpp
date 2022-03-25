#include "SubspaceEmpExplosion.h"

SubspaceEmpExplosion::SubspaceEmpExplosion()
{
	double animationTime = 500;

	effectTexture_.load("graphics/empburst.bm2", false);
	effectTexture_.setFrameSize(80, 80);
	effectTexture_.setAnimationPeriod(animationTime);

	timeToLive_ = animationTime;
}

SubspaceEmpExplosion::~SubspaceEmpExplosion()
{
}
