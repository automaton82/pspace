#include "SubspaceTile.h"

TextureAnimated SubspaceTile::tilesTexture_;	//texture for all tiles
TextureAnimated SubspaceTile::doorHorizontalTexture_;
TextureAnimated SubspaceTile::doorVerticalTexture_;
TextureAnimated SubspaceTile::asteroidSmall1Texture_;
TextureAnimated SubspaceTile::asteroidSmall2Texture_;
TextureAnimated SubspaceTile::asteroidLargeTexture_;
TextureAnimated SubspaceTile::stationTexture_;
TextureAnimated SubspaceTile::wormholeTexture_;
TextureAnimated SubspaceTile::goalEnemyTexture_;
TextureAnimated SubspaceTile::goalTeamTexture_;

SubspaceTile::SubspaceTile() : 
	x_(0), y_(0), texture_(0)
{
	setType(0);
}

SubspaceTile::SubspaceTile(int x, int y, TileType type) :
	x_(x), y_(y), texture_(0)
{
	setType(type);
}

SubspaceTile::SubspaceTile(const SubspaceTile& copy) :
	x_(0), y_(0), texture_(0)
{
	*this = copy;
}

// TODO: get rid of this
SubspaceTile& SubspaceTile::operator =(const SubspaceTile& rhs)
{
	if(this != &rhs)
	{
		x_ = rhs.x_;
		y_ = rhs.y_;
		type_ = rhs.type_;

		texture_ = rhs.texture_;
		/*if(texture_)
		{
			delete texture_;
			texture_ = 0;
		}

        if(rhs.texture_)
		{
			texture_ = new TextureAnimated();
			*texture_ = *rhs.texture_;
		}*/
	}
	return *this;
}

SubspaceTile::~SubspaceTile()
{
	/*if(texture_)
		delete texture_;*/
}

int SubspaceTile::getX() const
{
	return x_;
}

int SubspaceTile::getY() const
{
	return y_;
}

SubspaceTile::TileType SubspaceTile::getType() const
{
	return type_;
}

void SubspaceTile::setType(TileType type)
{
	if(type < vieNormalEnd)
	{
		//nothing;
	}
	else if(type >= vieVDoorStart && type <= vieVDoorEnd)
		texture_ = &doorVerticalTexture_;
	else if(type >= vieHDoorStart && type <= vieHDoorEnd)
		texture_ = &doorHorizontalTexture_;		
	else if(type >= vieAsteroidStart && type <= vieAsteroidEnd)
	{
		switch(type)
		{
		case vieAsteroidStart+0:
			texture_ = &asteroidSmall1Texture_;
			break;
		case vieAsteroidStart+1:
			texture_ = &asteroidLargeTexture_;
			break;
		case vieAsteroidStart+2:
			texture_ = &asteroidSmall2Texture_;
		default:
			texture_ = &asteroidSmall1Texture_;
			break;		
		}
	}
	else if(type == vieStation)
		texture_ = &stationTexture_;	
	else if(type == vieWormhole)
		texture_ = &wormholeTexture_;	
	else if(type == vieGoalArea || type == ssbEnemyGoal)
		texture_ = &goalEnemyTexture_;		
	else if(type == ssbTeamGoal)
		texture_ = &goalTeamTexture_;

	/*if(type < vieNormalEnd)
	{
		//nothing;
	}
	else if(type >= vieVDoorStart && type <= vieVDoorEnd)
	{
		texture_ = new TextureAnimated();
		*texture_ = tilesTexture_;	//copy main texture
		texture_->setFrameRange(vieVDoorStart-1, vieVDoorEnd);
		texture_->setAnimationPeriod(500);
	}
	else if(type >= vieHDoorStart && type <= vieHDoorEnd)
	{
		texture_ = new TextureAnimated();
		*texture_ = tilesTexture_;	//copy main texture
		texture_->setFrameRange(vieHDoorStart-1, vieHDoorEnd);
		texture_->setAnimationPeriod(500);
	}
	else if(type >= vieAsteroidStart && type <= vieAsteroidEnd)
	{
		texture_ = new TextureAnimated();

		switch(type)
		{
		case vieAsteroidStart+0:
			(*texture_).load("graphics/over1.bm2");
			texture_->setFrameSize(16, 16);
			break;
		case vieAsteroidStart+1:
			(*texture_).load("graphics/over2.bm2");
			texture_->setFrameSize(32, 32);
			break;
		case vieAsteroidStart+2:
			(*texture_).load("graphics/over3.bm2");
			texture_->setFrameSize(16, 16);
			break;
		}
		texture_->setAnimationPeriod(1000);
	}
	else if(type == vieStation)
	{
		texture_ = new TextureAnimated();
		(*texture_).load("graphics/over4.bm2");
		texture_->setFrameSize(96, 96);
		texture_->setAnimationPeriod(1000);
	}
	else if(type == vieWormhole)
	{
		texture_ = new TextureAnimated();
		(*texture_).load("graphics/over5.bm2");
		texture_->setFrameSize(80, 80);
		texture_->setAnimationPeriod(1000);
	}
	else if(type == vieGoalArea || type == ssbEnemyGoal)
	{
		texture_ = new TextureAnimated();
		(*texture_).load("graphics/goal.bm2");
		texture_->setFrameSize(16, 16);
		texture_->setFrameRange(9, 18);
		texture_->setAnimationPeriod(500);
	}
	else if(type == ssbTeamGoal)
	{
		texture_ = new TextureAnimated();
		(*texture_).load("graphics/goal.bm2");
		texture_->setFrameSize(16, 16);
		texture_->setFrameRange(0, 9);
		texture_->setAnimationPeriod(500);
	}*/
    		
	type_ = type;
}

bool SubspaceTile::isAsteroid() const
{
	return (type_ >= vieAsteroidStart && type_ <= vieAsteroidEnd);
}

bool SubspaceTile::isAsteroidPart() const
{
	return (type_ == ssbAsteroidPart);
}

bool SubspaceTile::isBigAsteroid() const
{
	return (type_ == vieBigAsteroid);
}

bool SubspaceTile::isBrick() const
{
	return (type_ == ssbTeamBrick || type_ == ssbEnemyBrick);		
}


bool SubspaceTile::isCollidable() const
{
	ExtendedTileType t = (ExtendedTileType)type_;

	if(t == vieNoTile)
		return false;
	else if(t > vieNoTile && t <= vieNormalEnd)
		return true;
	/*else if(t >= vieDoorStart && t <= vieDoorEnd)		//TODO: only until doors turn off
		return true;*/
	else if(t >= vieSpecialStart && t <= vieSpecialEnd)
		return false;
	else if(t >= vieAsteroidStart && t <= vieAsteroidEnd)
		return true;
	else if(t == vieStation)
		return true;
	else if(t == ssbEnemyBrick)
		return true;
	else if(t == ssbAsteroidPart)
		return true;
	else 
		return false;
}

bool SubspaceTile::isDoor() const
{
	return (type_ >= vieDoorStart && type_ <= vieDoorEnd);
}

bool SubspaceTile::isEmpty() const
{
	return (type_ == vieNoTile);
}

bool SubspaceTile::isGoal() const
{
	return(type_ == vieGoalArea ||
		type_ == ssbTeamGoal || type_ == ssbEnemyGoal);		
}

bool SubspaceTile::isTeamGoal() const
{
	return (type_ == ssbTeamGoal);		
}

bool SubspaceTile::isEnemyGoal() const
{
	return (type_ == ssbEnemyGoal);
}

bool SubspaceTile::isNormal() const
{
	return (type_ > vieNoTile && type_ <= vieNormalEnd);		
}

bool SubspaceTile::isOver() const
{
	return (type_ >= vieFlyOverStart && type_ <= vieFlyOverEnd);
}

bool SubspaceTile::isSafe() const
{
	return (type_ == vieSafeZone);		
}

bool SubspaceTile::isUnder() const
{
	return (type_ >= vieFlyUnderStart && type_ <= vieFlyUnderEnd);		
}

bool SubspaceTile::isWormhole() const
{
	return (type_ == vieWormhole);
}

void SubspaceTile::initTextures()
{
	static bool loaded = false;
	if(loaded)
		return;

	doorHorizontalTexture_ = tilesTexture_;
	doorHorizontalTexture_.setFrameRange(vieHDoorStart-1, vieHDoorEnd);
	doorHorizontalTexture_.setAnimationPeriod(500);

	doorVerticalTexture_ = tilesTexture_;
	doorVerticalTexture_.setFrameRange(vieVDoorStart-1, vieVDoorEnd);
	doorVerticalTexture_.setAnimationPeriod(500);
	
	asteroidSmall1Texture_.load("graphics/over1.bm2");
	asteroidSmall1Texture_.setFrameSize(16, 16);
	asteroidSmall1Texture_.setAnimationPeriod(1000);

	asteroidLargeTexture_.load("graphics/over2.bm2");
	asteroidLargeTexture_.setFrameSize(32, 32);
	asteroidLargeTexture_.setAnimationPeriod(1000);

	asteroidSmall2Texture_.load("graphics/over3.bm2");
	asteroidSmall2Texture_.setFrameSize(16, 16);
	asteroidSmall2Texture_.setAnimationPeriod(1000);

	stationTexture_.load("graphics/over4.bm2");
	stationTexture_.setFrameSize(96, 96);
	stationTexture_.setAnimationPeriod(1000);

	wormholeTexture_.load("graphics/over5.bm2");
	wormholeTexture_.setFrameSize(80, 80);
	wormholeTexture_.setAnimationPeriod(1000);

	goalEnemyTexture_.load("graphics/goal.bm2");
	goalEnemyTexture_.setFrameSize(16, 16);
	goalEnemyTexture_.setFrameRange(9, 18);
	goalEnemyTexture_.setAnimationPeriod(500);

	goalTeamTexture_.load("graphics/goal.bm2");
	goalTeamTexture_.setFrameSize(16, 16);
	goalTeamTexture_.setFrameRange(0, 9);
	goalTeamTexture_.setAnimationPeriod(500);
}

void SubspaceTile::setTilesTexture(const TextureAnimated& texture)
{
	tilesTexture_ = texture;
}

void SubspaceTile::draw() const
{
	//todo: add special drawing stuff here

	if(!isEmpty())
	{
		if(isDoor())
			glColor4d(1.0, 1.0, 1.0, 0.5);		//TODO: only until doors turn off
		else
			glColor4d(1.0, 1.0, 1.0, 1.0);

		if(texture_)
			texture_->display();
		else if(type_ <= vieSpecialEnd)
			tilesTexture_.displayFrame(type_ - 1);

		//tilesTexture_.displayFrame(1);
		//tilesTexture_.Texture::displayAll();
	}
}

void SubspaceTile::update(double time)
{
	if(texture_)
		texture_->advanceFrame(time);
}

void SubspaceTile::updateTextures(double time)
{
	doorHorizontalTexture_.advanceFrame(time);
	doorVerticalTexture_.advanceFrame(time);
	asteroidSmall1Texture_.advanceFrame(time);
	asteroidSmall2Texture_.advanceFrame(time);
	asteroidLargeTexture_.advanceFrame(time);
	stationTexture_.advanceFrame(time);
	wormholeTexture_.advanceFrame(time);
	goalEnemyTexture_.advanceFrame(time);
	goalTeamTexture_.advanceFrame(time);
}