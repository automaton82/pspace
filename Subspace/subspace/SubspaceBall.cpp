#include "SubspaceBall.h"

SubspaceBall::SubspaceBall(Uint ballID) :
	ballID_(ballID),
	isCarried_(false),
	timestamp_(-1),
	friction_(1)
{
	ballTexture_.load("graphics/powerb.bm2");
	ballTexture_.setFrameSize(16, 16);
	ballTexture_.setAnimationPeriod(1000);
	ballTexture_.setFrameRange(0, 10);
}

SubspaceBall::~SubspaceBall()
{
}

Uint SubspaceBall::getBallID() const
{
	return ballID_;
}

void SubspaceBall::setBallID(Uint id)
{
	ballID_ = id;
}

void SubspaceBall::setIsCarried(bool isCarried)
{
	isCarried_ = isCarried;
}

bool SubspaceBall::isCarried() const
{
	return isCarried_;
}

void SubspaceBall::setOwnerID(Uint id)
{
	ownerID_ = id;
}

Uint SubspaceBall::getOwnerID() const
{
	return ownerID_;
}

void SubspaceBall::setFriction(double friction)
{
	friction_ = friction;
}

Uint SubspaceBall::getTimestamp() const
{
	return timestamp_;
}

void SubspaceBall::setTimestamp(Uint timestamp)
{
	timestamp_ = timestamp;
}

void SubspaceBall::update(double time)
{
	ballTexture_.advanceFrame(time);

	SubspaceGameObject::update(time);

	//Uint16 maxFriction = ~0;
	//this->velocity_ *= 1.0 - friction_/(double)maxFriction * time;//1.0/friction_ * 1.0/time/10000;	//adjust speed based on friction
}

void SubspaceBall::draw() const
{
	ballTexture_.display();
}

void SubspaceBall::display() const
{
	if(!isCarried())
		SubspaceGameObject::display();
}
