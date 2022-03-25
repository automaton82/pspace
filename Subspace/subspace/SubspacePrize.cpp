#include "SubspacePrize.h"


SubspacePrize::SubspacePrize() : 
	type_(PRIZE_Unknown)
{
	prizeTexture_.load("graphics/prizes.bm2");
	prizeTexture_.setFrameSize(16, 16);
	prizeTexture_.setAnimationPeriod(500);

}

SubspacePrize::SubspacePrize(double x, double y, PrizeType type) : 
	type_(type)
{
	position_.x_ = x;
	position_.y_ = y;

	prizeTexture_.load("graphics/prizes.bm2");
	prizeTexture_.setFrameSize(16, 16);
	prizeTexture_.setAnimationPeriod(500);
}

SubspacePrize::~SubspacePrize()
{
}

void SubspacePrize::setPrizeType(PrizeType type)
{
	type_ = type;
}

void SubspacePrize::setTimeToLive(double time)
{
	timeToLive_ = time;
}

PrizeType SubspacePrize::getPrizeType() const
{
	return type_;
}

void SubspacePrize::update(double time)
{
	prizeTexture_.advanceFrame(time);

	timeToLive_ -= time;
	if(timeToLive_ <= 0)
		garbage = true;
}

void SubspacePrize::draw() const
{
	prizeTexture_.display();
}