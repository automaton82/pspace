#include "SubspaceExhaust.h"

#include "Color.h"

static double exhaustAnimationTime = 500;

SubspaceExhaust::SubspaceExhaust(bool rocket) :
	isRocket_(rocket), started_(false)
{
	if(isRocket_)
	{
		effectTexture_.load("graphics/rocket.bm2", false);
		effectTexture_.setFrameSize(24, 24);
		effectTexture_.setAnimationPeriod(exhaustAnimationTime * 26.0/20.0);
	}
	else
	{
		effectTexture_.load("graphics/exhaust.bm2", false);
		effectTexture_.setFrameSize(16, 16);
		effectTexture_.setAnimationPeriod(exhaustAnimationTime);
		effectTexture_.setFrameRange(0, 19);
	}

	timeToLive_ = exhaustAnimationTime;
	startTime_ = 0;
}

SubspaceExhaust::~SubspaceExhaust()
{
}

void SubspaceExhaust::draw() const
{
	double alpha = 1;//(timeToLive_ / exhaustAnimationTime);
	Color c(1.0, 1.0, 1.0, alpha);
	c.enable();

	SubspaceSpecialEffect::draw();
}


void SubspaceExhaust::update(double time)
{
	double increase = 1.0;

	if(!started_)
	{
		startVelocity_ = getVelocity();
		startTime_ = timeToLive_;
		started_ = true;
	}
	SubspaceSpecialEffect::update(time);

	//slowdown
	//linear
	//double scale = -2.0*(1.0 - timeToLive_/exhaustAnimationTime) + 1.0;	//should be 0 at 1/2 the animation time

	//quadratic
	double x = 1.0 - timeToLive_/exhaustAnimationTime;
	double scale = -8.0 * x*x + 1.0;

	if(scale < 0)
		scale = 0;
	velocity_ = scale * startVelocity_;

	/*if(timeToLive_ / startTime_ < 1.0 - 1.0/4.0)
		velocity_ = Vector(0,0,0);*/
}
