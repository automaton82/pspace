#include "SubspacePlayerAdapter.h"

#include "SubspacePlayer.h"
#include "SubspaceZone.h"

SubspacePlayerAdapter::SubspacePlayerAdapter() : 
	zone_(0)
{

}
	
void SubspacePlayerAdapter::setZone(SubspaceZone* zone)
{
	zone_ = zone;
}

///////////////////////////////////////
#define getPlayer()										\
	if(!zone_)											\
		return;											\
	SubspacePlayer* player = zone_->getMyPlayer();		\
	if(!player)											\
		return;

#define playerFireWeapon(name)							\
	if(!zone_)											\
		return;											\
	zone_->playerFire##name(zone_->getMyPlayerID());	

///////////////////////////////////////

void SubspacePlayerAdapter::doFireBrick()	
{
	getPlayer();
	player->useBrick();
}

void SubspacePlayerAdapter::doFireBomb()	{ playerFireWeapon(Bomb); }
void SubspacePlayerAdapter::doFireBullet()	{ playerFireWeapon(Bullet); }
void SubspacePlayerAdapter::doFireBurst()	{ playerFireWeapon(Burst); }
void SubspacePlayerAdapter::doFireDecoy()	{ playerFireWeapon(Decoy); }
void SubspacePlayerAdapter::doFireMine()	{ playerFireWeapon(Mine); }
void SubspacePlayerAdapter::doFirePortal()	{ playerFireWeapon(Portal); } 
void SubspacePlayerAdapter::doFireRepel()	{ playerFireWeapon(Repel); }

void SubspacePlayerAdapter::doFireRocket()	
{ 
	getPlayer();
	player->useRocket();
}

void SubspacePlayerAdapter::doFireThor()	{ playerFireWeapon(Thor); }


void SubspacePlayerAdapter::doMoveForward()
{
	getPlayer();
	player->thrust();
}

void SubspacePlayerAdapter::doMoveBackward()
{
	getPlayer();
	player->thrustReverse();
}

void SubspacePlayerAdapter::doTurnLeft()
{
	getPlayer();
	player->turnLeft();
}

void SubspacePlayerAdapter::doTurnRight()
{
	getPlayer();
	player->turnRight();
}

void SubspacePlayerAdapter::doFastForward() 
{
	getPlayer();
	player->thrust(true);
}

void SubspacePlayerAdapter::doFastBackward() 
{
	getPlayer();
	player->thrustReverse(true);
}

void SubspacePlayerAdapter::doFastLeft()
{
	getPlayer();
	player->turnLeft(true);
}

void SubspacePlayerAdapter::doFastRight()
{
	getPlayer();
	player->turnRight(true);
}

void SubspacePlayerAdapter::doToggleAntiwarp() 
{
	getPlayer();
	player->toggleAntiwarp();
}

void SubspacePlayerAdapter::doToggleCloak() 
{
	getPlayer();
	player->toggleCloak();
}

void SubspacePlayerAdapter::doToggleMultifire()
{
	getPlayer();
	player->toggleMultifire();
}

void SubspacePlayerAdapter::doToggleStealth() 
{
	getPlayer();
	player->toggleStealth();
}

void SubspacePlayerAdapter::doToggleXRadar() 
{
	getPlayer();
	player->toggleXRadar();
}

void SubspacePlayerAdapter::doAttach() {}	//turret attachment
void SubspacePlayerAdapter::doWarp() 
{
	getPlayer();
	if(player->getPortalTime() > 0)		//TODO: move this into player?
		player->usePortal();
	else
		player->warp(true);		// TODO: implement anti-warp stuff	
}

void SubspacePlayerAdapter::doChangeShip(int shipID) 
{
	if(!zone_)
		return;
	zone_->playerRequestShipChange(zone_->getMyPlayerID(), (ShipType)shipID);
}

void SubspacePlayerAdapter::doTeamChange(int teamID)
{
	if(!zone_)
		return;
	zone_->playerChangeTeam(zone_->getMyPlayerID(), teamID);
}
