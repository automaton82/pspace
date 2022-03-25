#include "SubspaceBrick.h"

#include "SubspaceZone.h"
#include "TextureManager.h"

SubspaceBrick::SubspaceBrick(Uint team, Uint myTeam)
{
	double animationTime = 500;

	brickTexture_.load("graphics/wall.bm2", false);
	brickTexture_.setFrameSize(16, 16);
	brickTexture_.setAnimationPeriod(animationTime);
	brickTexture_.setFrameRange(10, 20);

	setTeam(team);
	setMyTeam(myTeam);
}

SubspaceBrick::~SubspaceBrick()
{
	getZone()->removeBrick(this);
}

Uint SubspaceBrick::getTeam() const
{
	return team_;
}

void SubspaceBrick::setTeam(Uint team)
{
	team_ = team;

	setTexture();
}

void SubspaceBrick::setMyTeam(Uint myTeam)
{
	myTeam_ = myTeam;

	setTexture();
}

void SubspaceBrick::setTexture()
{
	if(team_ == myTeam_)
		brickTexture_.setFrameRange(10, 20);
	else
		brickTexture_.setFrameRange(0, 10);
}

void SubspaceBrick::update(double time)
{
	brickTexture_.advanceFrame(time);

	SubspaceWeapon::update(time);
}

void SubspaceBrick::draw() const
{
	brickTexture_.display();
}
