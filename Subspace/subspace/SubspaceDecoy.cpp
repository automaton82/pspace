#include "SubspaceDecoy.h"

#include "TextureManager.h"

#include "SubspacePlayer.h"
#include "SubspaceZone.h"

SubspaceDecoy::SubspaceDecoy(SubspacePlayer* player) : 
	player_(player)
{
	setPlayer(player);
}

SubspaceDecoy::~SubspaceDecoy()
{
}

void SubspaceDecoy::setPlayer(SubspacePlayer* player)
{
	player_ = player;

	decoyTexture_.load("graphics/ships.bm2", false);
	decoyTexture_.setFrameSize(36, 36);
	decoyTexture_.setFrameRange(0, 40);

	this->setTexture(decoyTexture_);

	lastDir_ = player_->getSubspaceDirection();
	oppositeDir_ = lastDir_;
}

WeaponData SubspaceDecoy::getWeaponSettings() const
{
	WeaponData w;
	w.u = 0;
	w.type = WEP_Decoy;
	
	return w;
}

bool SubspaceDecoy::isBouncy() const
{
	return true;
}

void SubspaceDecoy::draw() const
{
	SubspaceGameObject::draw();
}

void SubspaceDecoy::update(double time)
{
	//copytexture
	Uint shipType = player_->getShipType();
	int dir = player_->getSubspaceDirection();
	int diff = dir - lastDir_;

	oppositeDir_ -= diff;
	oppositeDir_ = (oppositeDir_ + 40 ) % 40;	//put in correct range;

	decoyTexture_.setFrameRange(40*shipType, 40*shipType + 40);
	decoyTexture_.setFrameInRange(oppositeDir_);	//opposite direction

	this->setTexture(decoyTexture_);

	SubspaceWeapon::update(time);

	lastDir_ = dir;
}
