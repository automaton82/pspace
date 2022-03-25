#include "SubspaceFlag.h"

SubspaceFlag::SubspaceFlag(Uint flagID) :
	isActive_(true),
	team_(-1),
	carrier_(0),
	flagID_(flagID)
{
	flagTexture_.load("graphics/flag.bm2", false);
	flagTexture_.setFrameSize(16, 16);
	flagTexture_.setAnimationPeriod(1000);
	flagTexture_.setFrameRange(0, 10);
}

SubspaceFlag::~SubspaceFlag()
{
	//clearTexture();
}

SubspacePlayer* SubspaceFlag::getCarrier() const
{
	return carrier_;
}

Uint SubspaceFlag::getFlagID() const
{
	return flagID_;
}

Uint SubspaceFlag::getTeam() const
{
	return team_;
}

void SubspaceFlag::setCarrier(SubspacePlayer* p)
{
	carrier_ = p;
}

void SubspaceFlag::setFlagID(Uint id)
{
	flagID_ = id;
}

void SubspaceFlag::setIsActive(bool b)
{
	isActive_ = b;
}

void SubspaceFlag::setIsMyTeamFlag(bool b)
{
	isMyTeamFlag_ = b;

	updateTexture();
}

void SubspaceFlag::setTeam(Uint team)
{
	team_ = team;
}

bool SubspaceFlag::isActive() const
{
	return isActive_;
}

bool SubspaceFlag::isCarried() const
{
	return (carrier_ != 0);
}

bool SubspaceFlag::isMyTeamFlag() const
{
	return isMyTeamFlag_;
}

void SubspaceFlag::update(double time)
{
	flagTexture_.advanceFrame(time);
}

void SubspaceFlag::draw() const
{
	if(!isCarried() && isActive())
		flagTexture_.display();
}


/*void SubspaceFlag::display() const
{
	if(!isCarried())
		SubspaceGameObject::display();
}*/

void SubspaceFlag::updateTexture()
{
	if(isMyTeamFlag_)
		flagTexture_.setFrameRange(10, 20);		//yellow flag
	else
		flagTexture_.setFrameRange(0, 10);		//blue flag
}

