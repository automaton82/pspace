#include "SubspaceZone.h"

#include <algorithm>
#include <fstream>

#include "GLUtil.h"
#include "TimeProfiler.h"
#include "zlib.h"

static TimeProfiler& timeProfiler = *TimeProfiler::getInstance();

#include "SubspaceBullet.h"
#include "SubspaceBall.h"
#include "SubspaceBrick.h"
#include "SubspaceBurstBullet.h"
#include "SubspaceDecoy.h"
#include "SubspaceEmpExplosion.h"
#include "SubspaceExhaust.h"
#include "SubspaceExplosion.h"
#include "SubspaceExplosionSmall.h"
#include "SubspaceFlag.h"
#include "SubspaceMultiBullet.h"
#include "SubspacePlayer.h"
#include "SubspacePortal.h"
#include "SubspaceRadar.h"
#include "SubspaceRepel.h"
#include "SubspaceShipExplosion.h"
#include "SubspaceShrapnel.h"
#include "SubspaceThor.h"

#include "SubspaceGlobal.h"
#include "SubspaceSound.h"
#include "SubspaceUtil.h"


SubspaceZone::SubspaceZone() :
	positionThread_(positionThreadRoutine),

	lvzManager_(0)
{
	numPrizes_ = 0;
}

SubspaceZone::~SubspaceZone()
{
	connected_ = false;
	positionThread_.stop(0);
}


void SubspaceZone::init()
{	
	background_.init();
}

void SubspaceZone::resize()
{
	background_.resize();
}

void SubspaceZone::onEnterArena()
{
	if(lvzManager_)
		lvzManager_->activateDisplayMode(LVZ_DISPLAYMODE_EnterArena);
}
void SubspaceZone::onEnterZone()
{
	if(lvzManager_)
		lvzManager_->activateDisplayMode(LVZ_DISPLAYMODE_EnterZone);
}

void SubspaceZone::initArena()
{
	players_.clear();

	initFlags();
}

bool SubspaceZone::loadSettings(const string& filename)
{
	/*DynamicData dataFile;
	bool result = false;
	
	result = dataFile.load(filename);
	if(!result)
		return false;
			
	//TODO: finish implementation, from file
	arenaSettings_.ActivateAppShutdownTime = dataFile["ActivateAppShutdownTime"];
	*/

	loadDefaultSettings();

	return false;
}

void SubspaceZone::loadDefaultSettings()
{
	for(int i=0; i < NUM_SHIPS; ++i)
	{
		arenaSettings_.ships[i].SeeBombLevel = 0;
		arenaSettings_.ships[i].Radius = 0;
		arenaSettings_.ships[i].SuperTime = 4500;
		arenaSettings_.ships[i].ShieldsTime = 6000;
		arenaSettings_.ships[i].Gravity = 1500;
		arenaSettings_.ships[i].GravityTopSpeed = 100;
		arenaSettings_.ships[i].BulletSpeed = 4100;
		arenaSettings_.ships[i].BombSpeed = 4800;
		arenaSettings_.ships[i].BulletFireDelay = 7;
		arenaSettings_.ships[i].MultiFireDelay = 7;
		arenaSettings_.ships[i].BombFireDelay = 7;
		arenaSettings_.ships[i].LandmineFireDelay = 125;
		arenaSettings_.ships[i].MultiFireAngle = 333;
		arenaSettings_.ships[i].MaximumRotation = 345;
		arenaSettings_.ships[i].MaximumThrust = 40;
		arenaSettings_.ships[i].MaximumSpeed = 3250;
		arenaSettings_.ships[i].MaximumRecharge = 12000;
		arenaSettings_.ships[i].MaximumEnergy = 1700;
		arenaSettings_.ships[i].InitialRotation = 345;
		arenaSettings_.ships[i].InitialThrust = 40;
		arenaSettings_.ships[i].InitialSpeed = 3250;
		arenaSettings_.ships[i].InitialRecharge = 12000;
		arenaSettings_.ships[i].InitialEnergy = 1;
		arenaSettings_.ships[i].BombThrust = 100;
		arenaSettings_.ships[i].BurstSpeed = 4300;
		arenaSettings_.ships[i].BurstShrapnel = 36;
		arenaSettings_.ships[i].RocketTime = 1000;
		arenaSettings_.ships[i].InitialBounty = 200;
		
		
		arenaSettings_.ships[i].BrickMax = 10;
		arenaSettings_.ships[i].BurstMax = 254;
		arenaSettings_.ships[i].DecoyMax = 10;
		arenaSettings_.ships[i].MaxMines = 5;
		arenaSettings_.ships[i].PortalMax = 10;
		arenaSettings_.ships[i].RepelMax = 10;
		arenaSettings_.ships[i].RocketMax = 10;
		arenaSettings_.ships[i].ThorMax = 10;		

		arenaSettings_.ships[i].InitialBombs = 3;
		arenaSettings_.ships[i].InitialBrick = 3;
		arenaSettings_.ships[i].InitialBurst = 254;
		arenaSettings_.ships[i].InitialDecoy = 3;
		arenaSettings_.ships[i].InitialGuns = 3;
		arenaSettings_.ships[i].InitialPortal = 3;
		arenaSettings_.ships[i].InitialRepel = 3;
		arenaSettings_.ships[i].InitialRocket = 3;
		arenaSettings_.ships[i].InitialThor = 3;
		
	}

	arenaSettings_.BulletAliveTime = 80;
	arenaSettings_.BombAliveTime = 1500;
	arenaSettings_.BounceFactor = 26;
	arenaSettings_.DecoyAliveTime = 6000;
	arenaSettings_.MineAliveTime = 1500;
	/*arenaSettings_.PrizeDelay = 300;
	arenaSettings_.PrizeFactor = 2000;
	arenaSettings_.PrizeHideCount = 3;*/
	arenaSettings_.RocketSpeed = 10000;
	arenaSettings_.RocketThrust = 100;
	arenaSettings_.WarpPointDelay = 10000;

	//arenaSettings_.MaxP
}

const ArenaSettings& SubspaceZone::getArenaSettings() const
{
	return arenaSettings_;
}

void SubspaceZone::setLogin(const string& username, const string& password)
{
	username_ = username;
	password_ = password;
}

void SubspaceZone::setStartArena(const string& arenaName)
{
	startArena_ = arenaName;
}

void SubspaceZone::setStartShip(ShipType ship)
{
	startShip_ = ship;
}

void SubspaceZone::dropBrick(Uint team, Uint x1, Uint y1, Uint x2, Uint y2, Uint timestamp, bool alert)
{
	for(Uint i=x1; i <= x2; ++i)
	{
		for(Uint j=y1; j <= y2; ++j)
		{
			SubspaceBrick* brick = new SubspaceBrick(team, getMyPlayer()->getTeam());
			brick->setZone(this);
			brick->setOwner(0);		//doesn't matter
			brick->setPosition(Vector(i * SubspaceMap::tileWidth, j * SubspaceMap::tileHeight, 0));
			brick->setTimeToLive(arenaSettings_.BrickTime * 10);
			brick->setTeam(team);

			//valid drop locations are returned as dropBrick packets from the server
			if(alert)	
			{	
				// TODO: implement
				// sendBrickDrop(brick->getSubspaceTileX(), brick->getSubspaceTileY());
				delete brick;
			}
			else	//create brick
			{			
				addBrick(brick);
				bool myTeam = (team == getMyPlayer()->getTeam());
				map_.setBrick(i, j, myTeam);
			}
		}
	}
}

void SubspaceZone::removeBrick(SubspaceBrick* brick)
{
	brick->setGarbage(true);

    map_.clearBrick(brick->getTileX(), brick->getTileY());
}

void SubspaceZone::updateBricks()
{
	BrickList::iterator i;
	for(i = bricks_.begin(); i != bricks_.end(); ++i)
	{
		SubspaceBrick* brick = (*i);

		Uint tileX = brick->getSubspaceTileX();
		Uint tileY = brick->getSubspaceTileY();

		map_.clearBrick(tileX, tileY);
		if(brick->getTeam() != getMyPlayer()->getTeam())
		{
			map_.setBrick(tileX, tileY, false);
		}
		else
		{
			map_.setBrick(tileX, tileY, true);
		}

		//TODO: only do this if team changed?
		brick->setMyTeam(getMyPlayer()->getTeam());
	}
}

void SubspaceZone::initFlag(int flagID)
{
	if(flags_.find(flagID) != flags_.end())
	{
		debugout << "Flag " << flagID << " already exists." << endl;
	}
	else
	{
		debugout << "Created flag " << flagID << endl;

		flags_[flagID] = new SubspaceFlag();
		flags_[flagID]->setFlagID(flagID);
	}
}

void SubspaceZone::initFlags()
{
	//debugout << flags_.size() << " flags: ";
	//debugout.write(printfString("%d flags: ", flags_.size()));

	FlagMap::iterator i;
	for(i = flags_.begin(); i != flags_.end(); ++i)
	{
		//debugout << (*i).first << " ";
		//debugout.write(printfString("%d ", (*i).first));
	}
	debugout << "." << endl;
}


void SubspaceZone::setFlag(int flagID, int team, const Vector& position)
{
	//TODO: any checks needed when setting position?
	FlagMap::const_iterator i;
	i = flags_.find(flagID);

	if(i == flags_.end())	//flagID doesn't exist, add new flag
		initFlag(flagID);

	SubspaceFlag* flag = flags_[flagID];
	assert(flag);

	flag->setTeam(team);
	flag->setPosition(position);
	/*if(getMyPlayer())
		flag->setMyTeam(getMyPlayer()->getTeam());*/

	//map_.removeObject(flag);
	map_.addObject(flag->getPosition().x_/SubspaceMap::tileWidth, flag->getPosition().y_/SubspaceMap::tileHeight, flag, true);
}


bool SubspaceZone::loadMap(const string& filename)
{
	bool loaded = map_.load(filename);

	/*LoadMapData data(&map_, filename);

	bool loaded = true;
	loadMapThread_.start(&data);*/

	/*if(loaded)
		hud_.setMap(&map_);*/

	return loaded;
}


const SubspaceMap* SubspaceZone::getMap() const
{
	return &map_;
}

void SubspaceZone::setZonePath(const string& path)
{
	zonePath_ = path;
}

bool SubspaceZone::saveBuffer(const char* filename, unsigned char* buffer, unsigned long size)
{
	std::ofstream file;
	
	file.open(filename, std::ios::binary);
	if(!file)
	{
		return false;
	}

	file.write((char*)buffer, size);
	file.close();

	return true;
}

bool SubspaceZone::uncompressBuffer(unsigned char* compressedBuffer, unsigned long compressedSize, unsigned char** outBuffer, unsigned long* outSize)
{
	//this->log("Processing map file: %s [%d]", mapName.c_str(), data.size());
	//zlib stuff
	int status = 0;
	uLongf length = compressedSize;

	do
	{
		if(*outBuffer)
			delete [] *outBuffer;

		length *= 2;
		*outBuffer = new unsigned char[length];
		
		status = uncompress(*outBuffer, &length, compressedBuffer, compressedSize);
	} 
	while(status == Z_BUF_ERROR);
	*outSize = length;

	if(status < Z_OK)
	{
		if(status == Z_DATA_ERROR)
		{
			printf("Map file \"%s\" corrupted.\n", mapFilename_.c_str());			
		}
		else
		{
            printf("Failed to decompress map file \"%s\".\n", mapFilename_.c_str());
			printf("Zlib error: %d\n", status);
		}
		
		//sendMapRequest();		
		return false;
	}

	return true;
}


void SubspaceZone::addBullet(SubspaceBullet* b)
{
	assert(b);

	bullets_.push_back(b);

	//displayManager_[WeaponLayer].addObject(b);
}

void SubspaceZone::addBomb(SubspaceBomb* b)
{
	assert(b);

	if(b->isProximityActive())
	{
		SubspacePlayerMap::iterator i;
		for(i = players_.begin(); i != players_.end(); ++i)
		{
			SubspacePlayer* player = (*i).second;
			assert(player);
		
			double maxRange = 200 * SubspaceMap::tileWidth;	//TODO: this is arbritrary, how do I get it from server?
			if( player->getTeam() != b->getOwner()->getTeam() &&	
				(player->getPosition() - b->getPosition()).magnitude() < maxRange)
			{
				b->addTarget(player);
			}
		}
	}

	bombs_.push_back(b);
	
	//displayManager_[WeaponLayer].addObject(b);
}

void SubspaceZone::addBrick(SubspaceBrick* brick)
{
	assert(brick);

	bricks_.push_back(brick);

	//displayManager_[MapItemLayer].addObject(brick);
}

void SubspaceZone::addPortal(SubspacePortal* portal)
{
	assert(portal);

	portals_.push_back(portal);

	//displayManager_[WeaponLayer].addObject(portal);
}

void SubspaceZone::addRepel(SubspaceRepel* repel)
{
	assert(repel);

	Uint team = repel->getOwner()->getTeam();

	BulletList::iterator i;
	for(i = bullets_.begin(); i != bullets_.end(); ++i)
	{
		SubspaceBullet* bullet = (*i);

		double dist = (bullet->getPosition() - repel->getPosition()).magnitude();
		if(bullet->getOwner()->getTeam() != team && 
			dist < arenaSettings_.RepelDistance)
			repel->addTarget(bullet);
	}

	BombList::iterator j;
	for(j = bombs_.begin(); j != bombs_.end(); ++j)
	{
		SubspaceBomb* bomb = (*j);

		double dist = (bomb->getPosition() - repel->getPosition()).magnitude();
		if(bomb->getOwner()->getTeam() != team &&
			dist < arenaSettings_.RepelDistance)
			repel->addTarget(bomb);
	}

	SubspacePlayerMap::iterator p;
	for(p = players_.begin(); p != players_.end(); ++p)
	{
		SubspacePlayer* player = (*p).second;
		if(player && player->getTeam() != team)
		{
			repel->addTarget(player);
		}
	}
	
	repels_.push_back(repel);

	//displayManager_[WeaponLayer].addObject(repel);
}

void SubspaceZone::addWeapon(SubspaceWeapon* wep)
{
	assert(wep);

	weapons_.push_back(wep);

	//displayManager_[WeaponLayer].addObject(wep);
}

void SubspaceZone::clearPortals(SubspacePlayer* owner)
{
	PortalList::iterator i;
	for(i = portals_.begin(); i != portals_.end(); ++i)
	{
		SubspacePortal* p = (*i);
		if(p->getOwner() == owner)
		{
			//displayManager_[WeaponLayer].removeObject(p);

			i = portals_.erase(i);	
			--i;						//TODO: is this necessary?
		}
	}
}


void SubspaceZone::destroyBullet(SubspaceBullet* b)
{
	BulletList::iterator i = std::find(bullets_.begin(), bullets_.end(), b);
	if(i != bullets_.end())
	{
		SubspaceBullet* bullet = (*i);

		//add explosion
		if(bullet->getDamage() != arenaSettings_.InactiveShrapnelDamage)	//don't explode inactive shrapnel
		{
			SubspaceExplosionSmall* exp = new SubspaceExplosionSmall();
			exp->setPosition(bullet->getPosition());
		
			addEffect(exp);
		}

		//printf("Bullet destroyed (%f,%f)\n", bullet->getPosition().x_, bullet->getPosition().y_);

		
		//effects_.push_back(exp);

		//delete bullet
		bullet->setGarbage(true);

		//displayManager_[WeaponLayer].removeObject(bullet);
	}
}

void SubspaceZone::destroyBomb(SubspaceBomb* b)
{
	BombList::iterator i = std::find(bombs_.begin(), bombs_.end(), b);
	if(i != bombs_.end())
	{
		SubspaceBomb* bomb = (*i);

		//add explosion
        SubspaceSpecialEffect* exp;
		if(b->isEmp())
			exp = new SubspaceEmpExplosion();
		else
			exp = new SubspaceExplosion();

		exp->setPosition(bomb->getPosition());

		addEffect(exp);
		//effects_.push_back(exp);

		//TODO: add shrapnel
		fireShrapnel(*b);
	
		//delete bomb
		bomb->setGarbage(true);

		//displayManager_[WeaponLayer].removeObject(bomb);
	}
}


void SubspaceZone::fireShrapnel(const SubspaceBomb& bomb)
{
	double angle = 0;
	double angleInc = 2*MathUtil::PI / bomb.getShrapnel();
	Vector vel;
    
	for(Uint i=0; i < bomb.getShrapnel(); ++i)
	{
		SubspaceShrapnel* newShrap = new SubspaceShrapnel(bomb.getShrapnelLevel(), bomb.getShrapnelBounce());

		newShrap->setOwner(bomb.getOwner());
		newShrap->setZone(bomb.getZone());
		newShrap->setTimeToLive(arenaSettings_.BulletAliveTime*10.0);
	
		newShrap->setDirection(bomb.getDirection());
		newShrap->setPosition(bomb.getPosition());

		if(arenaSettings_.RandomShrapnel)
			angle = MathUtil::unitRand() * 2*MathUtil::PI;
		
		vel.x_ = cos(angle);
		vel.y_ = sin(angle);
		vel.z_ = 0;
		vel *= arenaSettings_.ShrapnelSpeed;
		newShrap->setVelocity(vel);

		angle += angleInc;

		this->addBullet(newShrap);
	}
}

SubspacePlayer* SubspaceZone::getMyPlayer()
{
	SubspacePlayer* player = 0;
	SubspacePlayerMap::iterator i = players_.find(getMyPlayerID());
	if(i != players_.end())
		player = (*i).second;

	return player;
}


Uint SubspaceZone::getMyPlayerID() const
{
	//if(!connection_)
	/*	return -1;
	else
		return connection_->getSelfID();*/

	return myPlayerID_;
}


Uint SubspaceZone::getNumPlayers() const
{
	return (Uint)players_.size();
}

SubspacePlayer* SubspaceZone::getPlayer(int id)
{
	SubspacePlayer* player = 0;
	SubspacePlayerMap::iterator i = players_.find(id);

	if(i != players_.end())
		player = (*i).second;
	
	return player;
}

const SubspacePlayerMap* SubspaceZone::getPlayers() const
{
	return &players_;
}


void SubspaceZone::addPlayer(SubspacePlayer* player, int id)
{
	assert(player);
	assert(!players_[id]);	//make sure the player does not exist

	player->setZone(this);
	player->setArenaSettings(&this->arenaSettings_);

	players_[id] = player;
	//displayManager_[LAYER2].addObject(player);	//TODO: which layer exactly?

	if(id == getMyPlayerID())	//initializing self player
	{
		player->initShip();	
		//player->warp();

		// hud_.setPlayer(player);		//set hud for self
		//updateFlagTeams();			//set flags
	}

	/*hud_.addSystemMessage(player->getName() + " entering game.", COLOR_Green);

	hud_.initInfoBox(infoBoxMode_);*/
}


void SubspaceZone::removePlayer(int id)
{
	assert(players_[id]);

	SubspacePlayerMap::iterator i = players_.find(id);
	if(i == players_.end())	//player not found
		return;

	SubspacePlayer* player = players_[id];
	
	players_.erase(i);
	//displayManager_[ShipLayer].removeObject(player);

	//hud_.initInfoBox(infoBoxMode_);
}

void SubspaceZone::updatePlayer(SubspacePlayer* player, int id)
{
	assert(players_[id]);	//make sure player exists

	//players_.addItem(player, id);
	players_[id] = player;
		//dm.addItem(player);

	player->setZone(this);
	player->setArenaSettings(&this->arenaSettings_);

	if(id == getMyPlayerID())
	{
		player->initShip();
		//player->warp();
	}
}

void SubspaceZone::playerChangeShip(int playerID, ShipType ship)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(player)
	{
		/*if(player == getMyPlayer())
		{
			if(connection_->isConnected())
				connection_->sendShipChange(ship);
			else
				player->changeShip(ship);

			//player->initShip();
			//player->warp();
		}
		else
		{
			player->changeShip(ship);
		}*/

		player->changeShip(ship);
		
		////////////////////////////
		//TODO: clear player weapons
		////////////////////////////
	}
}

void SubspaceZone::playerChangeTeam(int playerID, int team)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;
	
	if(team == player->getTeam())
	{
		/*if(player == getMyPlayer())
			hud_.addSystemMessage("You are already on that frequency.", COLOR_Yellow);*/
	}
	else
	{
		//updateFlagTeams();
		player->changeTeam(team, player->getShipType());
		updateBricks();

		//hud_.refreshStatBox();
	}
}


void SubspaceZone::playerDeath(int playerID)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;

	if(player == getMyPlayer())
	{
		//TODO: finish death
	}

	player->die();

	SubspaceShipExplosion* newExp = new SubspaceShipExplosion();
	newExp->setPosition(player->getPosition());
	newExp->setVelocity(player->getVelocity());
	effects_.push_back(newExp);

	//displayManager_[LAYER2].addObject(newExp);
}


void SubspaceZone::playerDropFlag(int playerID, bool alertDrop)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(player)
	{
		if(alertDrop && player == getMyPlayer())
		{
			sendFlagsDrop();
		}

		player->dropFlags();
		//assert(flag && flags_[flag->getFlagID()] == flag);

		//add flags to map
		/*FlagMap::iterator i;
		for(i = flags_.begin(); i != flags_.end(); ++i)
		{
			SubspaceFlag* flag = (*i).second;

			flag->setMyTeam(getMyPlayer()->getTeam());
		}*/
	}
}

void SubspaceZone::playerFireBrick(int playerID)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;

	if(player == getMyPlayer())
	{
		if(player->useBrick())
		{
			sendBrickDrop(player->getSubspaceTileX(), player->getSubspaceTileY());
		}
	}
}

void SubspaceZone::playerFireBomb(int playerID)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;

	SubspaceBomb* bomb = player->fireBomb();
	if(!bomb)
		return;

	if(player == getMyPlayer())
	{
		sendWeapon(bomb->getWeaponSettings());	// TODO: this is a position packet 0x03 with weapon data
	}
}

void SubspaceZone::playerFireBullet(int playerID)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;

	SubspaceBullet* bullet = player->fireBullet();	//TODO: automatically adds bullet to zone, maybe shouldn't do that
	if(!bullet)
		return;

	if(player == getMyPlayer())
	{
		sendWeapon(bullet->getWeaponSettings());
	}
}

void SubspaceZone::playerFireBurst(int playerID)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;

	SubspaceBurstBullet* burst = player->fireBurst();
	if(!burst)
		return;

	if(player == getMyPlayer())
	{
		sendWeapon(burst->getWeaponSettings());
	}
}

void SubspaceZone::playerFireDecoy(int playerID)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;

	SubspaceDecoy* decoy = player->fireDecoy();
	if(!decoy)
		return;

	if(player == getMyPlayer())
	{
		sendWeapon(decoy->getWeaponSettings());
	}
}

void SubspaceZone::playerFireMine(int playerID)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;

	SubspaceBomb* bomb = player->fireMine();
	if(!bomb)
	{
		/*if(!player->hasMoreMines())
		{
			hud_.addSystemMessage("You have too many mines", COLOR_Yellow);
		}*/
		return;
	}

	if(player == getMyPlayer())
	{
		WeaponData w = bomb->getWeaponSettings();
		w.alternate = true;

		sendWeapon(w);
	}
}

void SubspaceZone::playerFirePortal(int playerID)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;

	SubspacePortal* portal = player->firePortal();
	if(portal)
	{
		//clear old player portals
		clearPortals(player);
		addPortal(portal);
	}
}

void SubspaceZone::playerFireRepel(int playerID)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;

	SubspaceRepel* repel = player->fireRepel();
	if(!repel)
		return;

	if(player == getMyPlayer())
	{
		sendWeapon(repel->getWeaponSettings());
	}
}

void SubspaceZone::playerFireThor(int playerID)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;

	SubspaceThor* thor = player->fireThor();
	if(!thor)
		return;

	if(player == getMyPlayer())
	{
		sendWeapon(thor->getWeaponSettings());
	}
}


void SubspaceZone::playerGotFlag(int playerID, int flagID)
{
	//TODO: any checks needed when setting position?

	if(flagID >= (int)flags_.size())
	{
		debugout << "Error: invalid flag id [" << flagID << "]" << endl;
		return;
	}
	
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;
		
	/*if(player == getMyPlayer())
	{
		connection_->sendFlagRequest(flagID);
	}*/

	if(flags_.find(flagID) == flags_.end())
	{
		initFlag(flagID);	
	}
	else
	{
		SubspaceFlag* flag = flags_[flagID];
			
		//remove flag from map
		map_.getTileObject(flag->getPosition().x_/SubspaceMap::tileWidth, flag->getPosition().y_/SubspaceMap::tileHeight);

		if(flag->isCarried())
			flag->getCarrier()->dropFlag(flag);

		flag->setIsActive(true);
		flag->setIsMyTeamFlag(player->getTeam() == getMyPlayer()->getTeam());
		player->giveFlag(flag);

		debugout << "Player [" << playerID << "] received flag [" << flagID << "]" << endl;
	}
}

void SubspaceZone::playerGotPrize(int playerID, PrizeType prize, Uint xTile, Uint yTile)
{
	//TODO: get real prize message
	//char buffer[255];
	//sprintf(buffer, "(PrizeMessage) Prize [%d] picked up at (%d,%d)", prize, xTile, yTile);
	//string message(buffer);
	string message = getPrizeMessage(prize);			//TODO: move this to player, account for "MAX"

	SubspacePlayer* player = getPlayer(playerID);
	SubspacePlayer* myPlayer = getMyPlayer();

	if(player && (player == myPlayer) )
	{
		//hud_.addSystemMessage(message, COLOR_Green);
		subspaceSounds[SOUND_Prize].play();

		//send prize to everyone else

		player->givePrize(prize);
	}
	else if(player)
	{
		player->givePrize(prize);
		
		if(myPlayer && player->getTeam() == myPlayer->getTeam() && 
			myPlayer->getBounty() < arenaSettings_.ships[myPlayer->getShipType()].PrizeShareLimit)
		{
			getMyPlayer()->givePrize(prize);
			//hud_.addSystemMessage(message, COLOR_Green);
		}
	}

	SubspaceGameObject* obj = map_.getTileObject(xTile, yTile);
	//displayManager_[MapItemLayer].removeObject(obj);
}

void SubspaceZone::playerKothEnded(int playerID)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(player == getMyPlayer())
	{
		sendKothTimerEnded();
	}
}


void SubspaceZone::playerRequestFlag(int playerID, int flagID)
{
	if(flagID >= (int)flags_.size())
		return;

	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;

	initFlag(flagID);	
	SubspaceFlag* flag = flags_[flagID];

	debugout << "Player [" << playerID << "] requested flag [" << flagID << "]" << endl;

	if(!flag->isActive() || flag->isCarried())
		return;
	
	if(player == getMyPlayer())
	{
		sendFlagRequest(flagID);
	}
	else		
	{
		flag->setIsActive(false);
	}
}

void SubspaceZone::playerRequestShipChange(int playerID, ShipType ship)
{
	if(!getMyPlayer())
		return;

	if(playerID == getMyPlayer()->getId())
	{
		sendShipChange(ship);
	}
}

void SubspaceZone::playerRequestTeamChange(int playerID, int team)
{
	if(!getMyPlayer())
		return;

	if(playerID == getMyPlayer()->getId())
	{
		sendTeamChange(team);		
	}
}


//powerball
void SubspaceZone::initBall(int ballID)
{
	if(balls_[ballID])
	{
		debugout << "Ball " << ballID << " already exists." << endl;
		//printf("Ball %d already exists.", ballID);
	}
	else
	{
		debugout << "Created ball " << ballID << endl;
		//printf("Created ball %d\n", ballID);
		balls_[ballID] = new SubspaceBall();
		balls_[ballID]->setBallID(ballID);

		//displayManager_[LAYER3].addObject(balls_[ballID]);
	}
}

void SubspaceZone::updateBalls()
{
	SubspacePlayer* p = getMyPlayer();

	BallMap::iterator i;
	/*for(i = balls_.begin(); i != balls_.end(); ++i)
	{
		SubspaceBall* ball = (*i).second;
		if(p)
			ball->setMyTeam(p->getTeam());
	}*/
}

void SubspaceZone::setBall(int ballID, int ownerID, const Vector& position, const Vector& velocity, Uint timestamp)
{
	BallMap::const_iterator i;
	i = balls_.find(ballID);

	if(i == balls_.end())				//ballID doesn't exist, add new ball
		initBall(ballID);

	SubspacePlayer* player = getPlayer(ownerID);

	//debugout << "Ball [" << ballID << "] carried by " << ownerID << ", at (" << position.x_ << "," << position.y_ << "):(" << velocity.x_ << "," << velocity.y_ << "); time: " << timestamp << endl;
	
	//debugout.write(printfString("Ball [%d] carried by %d, at (%f,%f):(%f,%f); time: %d\n", ballID, ownerID, position.x_, position.y_, velocity.x_, velocity.y_, timestamp));
	//printf("Ball [%d] carried by %d, at (%f,%f):(%f,%f); time: %d\n", ballID, ownerID, position.x_, position.y_, velocity.x_, velocity.y_, timestamp);
	//TODO: is this really how carrying is determined?

	if(player && timestamp == 0)	
	{					//carried ball
		balls_[ballID]->setPosition(position);
		balls_[ballID]->setVelocity(velocity);

		player->giveBall(balls_[ballID]);
	}
	else
	{			//uncarried ball
		SubspacePlayer* owner = getPlayer(balls_[ballID]->getOwnerID());
		if(owner)
		{
			owner->dropBall();
		}

		if(timestamp != balls_[ballID]->getTimestamp())
		{
			balls_[ballID]->setTimestamp(timestamp);

			balls_[ballID]->setPosition(position);
			balls_[ballID]->setVelocity(velocity);
		}

		//balls_[ballID]->setIsCarried(false);

		/*if(timestamp == 0)
			balls_[ballID]->setVelocity(Vector(0,0,0));*/
	}
}


void SubspaceZone::initPrizes()
{
	prizeGenerator_.setPopulation(getNumPlayers());
	
	this->prizeGenerator_.setPrizeWeights(arenaSettings_.prizeSettings, arenaSettings_.PrizeNegativeFactor);
	this->prizeGenerator_.setSpawnRange(arenaSettings_.PrizeMinimumVirtual, arenaSettings_.PrizeUpgradeVirtual);
	
	//TODO: make this an internally stored seed
	//this->prizeGenerator_.setSeed(this->connection_->getPrizeSeed());

	//TODO: check Tick count to millsecond conversion
	prizeGenerator_.setPrizeTimeToLive(arenaSettings_.PrizeMinExist * 10, arenaSettings_.PrizeMaxExist * 10);
}

void SubspaceZone::updatePrizes(double time)
{
	prizeGenerator_.setPopulation(getNumPlayers());

	//TODO: make this use an actual timer
	Uint16 delay = max(arenaSettings_.PrizeDelay, 1);
	int iterations = (GetTickCount() / 10 - lastPrizeUpdate_) / delay;
	for(int i=0; i < iterations; ++i)
	{
		for(int j=0; j < arenaSettings_.PrizeHideCount && getNumPrizes() < getNumPrizesMax(); ++j)
		//for(int j=0; j < getNumPrizes() < 1; ++j)
		{
			spawnPrize();			
		}
	}

	if(iterations > 0)
		lastPrizeUpdate_ = GetTickCount() / 10;

	//update prize list
	//updateList(prizes_, time);
}

void SubspaceZone::spawnPrize()
{
	spawnPrize(0);
}

void SubspaceZone::spawnPrize(Vector* pos)
{
	SubspacePrize* p = prizeGenerator_.createPrize();

	if(pos)
		p->setPosition(*pos);

	Uint x = (Uint)(p->getPosition().x_ / SubspaceMap::tileWidth);
	Uint y = (Uint)(p->getPosition().y_ / SubspaceMap::tileHeight);

	if(map_.addObject(x, y, p))
	{
		//displayManager_[MapItemLayer].addObject(p);

		//debugout << "Spawned prize [" << p->getPrizeType() << "] at (" << x << "," << y << ")" << endl;
		//printf("Spawned prize [%d] at (%d, %d)\n", p->getPrizeType(), x, y);
		++numPrizes_;
	}
	else
		delete p;
}


Uint SubspaceZone::getNumPrizes() const
{
	//return prizes_.size();

	return numPrizes_;
	//assert(0);

	return 0;
}


Uint SubspaceZone::getNumPrizesMax() const
{
	//TODO: clean this function up
	/*const Uint32 cosmicPrize = 0x10624DD3;

	long total = IMULHIDWORD(arenaSettings_.PrizeFactor * getNumPlayers(), cosmicPrize);
	total = (total >> 6) + (((Uint32)total) >> 31);
	total = min(total, 256);

	long created = total - getNumPrizes();

	return total;*/

	//return 1;
	return (Uint)(arenaSettings_.PrizeFactor / 1000.0 * getNumPlayers());
}


SubspacePrizeGenerator& SubspaceZone::getPrizeGenerator()
{
	return prizeGenerator_;
}

string SubspaceZone::getPrizeMessage(PrizeType prize) const
{
	switch(prize)
	{
		case PRIZE_Unknown:		return "Unknown prize picked up.";
		case PRIZE_Recharge:	return "Charge Rate Increased.";
		case PRIZE_Energy:		return "Maximum energy level increased.";
		case PRIZE_Rotation:	return "Rotation speed increased.";
		case PRIZE_Stealth:		return "Stealth available.";
		case PRIZE_Cloak:	return "Cloak available.";
		case PRIZE_XRadar:	return "X-Radar available.";
		case PRIZE_Warp:	return "Warp!";
		case PRIZE_Guns:	return "Guns upgraded.";
		case PRIZE_Bombs:	return "Bombs upgraded.";
		case PRIZE_BBullets:	return "Bouncing bullets.";
		case PRIZE_Thruster:	return "Thrusters upgraded.";
		case PRIZE_TopSpeed:	return "Top speed increased.";
		case PRIZE_FullCharge:	return "Full Charge.";
		case PRIZE_EngineShutdown:	return "Engine shutdown.";
		case PRIZE_Multifire:	return "Multifire.";
		case PRIZE_Proximity:	return "Proximity Bombs.";
		case PRIZE_Super:	return "Temporary SuperPower!";
		case PRIZE_Shields:	return "Temporary shields!";
		case PRIZE_Shrapnel:	return "Shrapnel increased.";
		case PRIZE_Antiwarp:	return "AntiWarp available.";
		case PRIZE_Repel:	return "Repeller increased.";
		case PRIZE_Burst:	return "Burst increased.";
		case PRIZE_Decoy:	return "Decoy increased.";
		case PRIZE_Thor:	return "Thor increased.";
		case PRIZE_Multiprize:	return "Multiprize!";
		case PRIZE_Brick:	return "Brick increased.";
		case PRIZE_Rocket:	return "Rocket increased.";
		case PRIZE_Portal:	return "Portal increased.";
	}

	return "";
}

void SubspaceZone::addEffect(SubspaceSpecialEffect* effect, bool isFar)
{
	if(isFar)
	{
		effectsFar_.push_back(effect);
		//displayManager_[EffectLayerFar].addObject(effect);
	}
	else
	{
		effects_.push_back(effect);
		//displayManager_[EffectLayer].addObject(effect);
	}
}

void SubspaceZone::updateBackground()
{
	//TODO: create a camera for the zone
	if(getMyPlayer())
	{
		//adjust for orthographic 
		background_.setCamera(getMyPlayer()->getPosition());
	}
}

void SubspaceZone::setLVZ(SubspaceLVZManager* lvzManager)
{	
	lvzManager_ = lvzManager;
}


void SubspaceZone::createTurret(int requesterID, int destinationID)
{
	SubspacePlayer* requester = getPlayer(requesterID);
	SubspacePlayer* destination = getPlayer(destinationID);

	if(destination)								//destination=0 if detaching
		destination->addTurret(requester);

	requester->setTurretParent(destination);
	
	debugout << "Turret link created: " << requesterID << " on " << destinationID << endl;
}

void SubspaceZone::destroyTurret(int playerID)
{
	SubspacePlayer* player = getPlayer(playerID);
	
	if(!player)
		debugout << "Invalid turret destruction: player [" << playerID << "]" << endl;
	else
	{
		debugout << "Destroy turret link on player [" << playerID << "]" << endl;
		//player->setTurretParent(0);	
		if(getMyPlayer()->getTurretParent() == player)
			requestTurret((Uint16)-1);

		player->clearTurrets();
	}
}

void SubspaceZone::requestTurret(int playerID)
{
	debugout << "Requested attach to player [" << playerID << "]" << endl;

	SubspacePlayer* p = getPlayer(playerID);
	if(p && p->getTeam() != getMyPlayer()->getTeam())
	{
		//hud_.addSystemMessage("Must attach to somebody of same frequency.", COLOR_Yellow);
		return;
	}
	else if(p == getMyPlayer())
	{
		//hud_.addSystemMessage("Cannot attach to self.", COLOR_Yellow);
	}

	sendAttachRequest(playerID);
}


void SubspaceZone::update(double timestep)
{
	timeProfiler.enter("players");

	//update players
	updateMap(players_, timestep);

	timeProfiler.exit("players");
	
	timeProfiler.enter("objects");
	//update objects
	updateList(bullets_, timestep, WeaponLayer);
	updateList(bombs_, timestep, WeaponLayer);
	updateList(portals_, timestep, WeaponLayer);
	updateList(repels_, timestep, WeaponLayer);
	updateList(weapons_, timestep, WeaponLayer);

	updateList(bricks_, timestep);

	timeProfiler.exit("objects");

	timeProfiler.enter("map");

	//update map
	map_.update(timestep);

	timeProfiler.exit("map");

	//update the hud
	//hud_.update(timestep);

	//update flags
	//updateFlagTeams();
	updateMap(flags_, timestep);
	updateMap(balls_, timestep);

	//update prizes - generate new ones
	updatePrizes(timestep);

	//update network stuff
	//updateNetworkState();

	timeProfiler.enter("background");

	//update special effects
	updateBackground();

	timeProfiler.exit("background");
	updateList(effects_, timestep, EffectLayer);
	updateList(effectsFar_, timestep, EffectLayerFar);

	timeProfiler.enter("collisions");

	//collisions
	handleMapObjectCollisions();
	handlePlayerObjectCollisions();

	timeProfiler.exit("collisions");

}

template <class ListType>
void SubspaceZone::updateList(ListType& list, double timeStep, ObjectLayerType layer)
{
	ListType::iterator i;
	ListType::value_type item;
	bool doDelete = false;

	for(i = list.begin(); i != list.end(); ++i)
	{
		item = (*i);
		if(item)
		{
			item->update(timeStep);
		}

        if(!item || item->isGarbage())
		{
			i = list.erase(i);
			--i;
		}

		if(item->isGarbage())
		{
			/*if(layer != UnknownLayer)
				displayManager_[layer].removeObject(item);*/
			delete item;		
			//item = 0;
		}
	}
}

template <class MapType>
void SubspaceZone::updateMap(MapType& map, double timeStep)
{
	MapType::iterator i;
	MapType::mapped_type item;
	bool doDelete = false;

	for(i = map.begin(); i != map.end(); ++i)
	{
		item = (*i).second;
		if(item)
		{
			item->update(timeStep);
		}

        if(!item || item->isGarbage())
		{
			i = map.erase(i);
			--i;
		}

		if(item->isGarbage())
		{
			delete item;		
			//item = 0;
		}
	}
}

/***********
 * Display *
 ***********/

void SubspaceZone::draw()
{
	const double z = 900;//800;
	Vector playerPos(512*16, 512*16, 0);	//initialize position at middle of map
	
	const SubspacePlayer* player;
	
	player = getMyPlayer();
	if(player)
	{
		playerPos = player->getPosition();		//set camera to look at player
	}

	timeProfiler.enter("background");

	//draw background
	background_.display();

	timeProfiler.exit("background");

	timeProfiler.enter("map");
	timeProfiler.enter("setup");

GLUtil::setOrthoMode(0, 0, subspaceGlobal.getWindowWidth(), subspaceGlobal.getWindowHeight(), -1000, 1000);
GLUtil::enterOrthoMode();

	Camera playerCamera;
    playerCamera.setPosition(Vector(playerPos.x_, playerPos.y_, z));
	playerCamera.setViewPoint(Vector(playerPos.x_, playerPos.y_, 0));

	glColor4f(1, 1, 1, 1);
	
	int drawWidth = 70;
	int drawHeight = drawWidth * 3 / 4;

	double xtemp = max(-1, (int)playerCamera.getPosition().x_/SubspaceMap::tileWidth*SubspaceMap::tileWidth-(double)(drawWidth*SubspaceMap::tileWidth));	//weird, but only draw tiles every tileWidth/height
	double ytemp = max(-1, (int)playerCamera.getPosition().y_/SubspaceMap::tileHeight*SubspaceMap::tileHeight-(double)(drawHeight*SubspaceMap::tileHeight));
	
	int xstart = max(-1, (int)playerCamera.getPosition().x_/16-drawWidth);
	int xend = max(-1, (int)playerCamera.getPosition().x_/16+drawWidth);
	int ystart = max(-1, (int)playerCamera.getPosition().y_/16-drawHeight);
	int yend = max(-1, (int)playerCamera.getPosition().y_/16+drawHeight);
	
	map_.setDrawRange(xstart, ystart, xend, yend);
	
	playerCamera.setPosition(playerCamera.getPosition() - Vector(subspaceGlobal.getWindowWidth()/2, subspaceGlobal.getWindowHeight()/2, 0));
	playerCamera.setViewPoint(playerCamera.getViewPoint() - Vector(subspaceGlobal.getWindowWidth()/2, subspaceGlobal.getWindowHeight()/2, 0));

	setScreenCamera(playerCamera.getPosition());

	for(Uint n=0; n < NumObjectLayers; ++n)
	{
        displayManager_[n].setCamera(playerCamera);
	}

	displayManager_[0].lookAtCamera();
	
	timeProfiler.exit("setup");
	timeProfiler.enter("over tiles");
	
	glPushMatrix();
		glTranslated( xtemp, ytemp, 0);
		map_.drawOverTiles();
	glPopMatrix();

	timeProfiler.exit("over tiles");
	timeProfiler.enter("basic tiles");

	glPushMatrix();
		glTranslated( xtemp, ytemp, 0);
		map_.drawStandardTiles();
	glPopMatrix();

	timeProfiler.exit("over tiles");

	timeProfiler.exit("map");

	//draw prizes
	//drawList(prizes_);

	//draw flags
	//drawMap(flags_);

	//displayManager_[MapItemLayer].display();

	timeProfiler.enter("objects");

	timeProfiler.enter("effects");
	drawList(effectsFar_);
	//displayManager_[EffectLayerFar].display();
	timeProfiler.exit("effects");

	//draw balls
	drawMap(balls_);

	//draw bricks
	drawList(bricks_);

	//draw objects
	drawList(bullets_);
	drawList(bombs_);
	drawList(portals_);
	drawList(repels_);
	drawList(weapons_);

	//displayManager_[WeaponLayer].display();

	//draw players
	drawMap(players_);

	
	//displayManager_[ShipLayer].display();

	//draw special effects
	timeProfiler.enter("effects");
	drawList(effects_);
	timeProfiler.exit("effects");

	//displayManager_[EffectLayer].display();

	timeProfiler.exit("objects");
	timeProfiler.enter("map");
	timeProfiler.enter("under tiles");

	glColor4f(1, 1, 1, 1);

	glPushMatrix();
		glTranslated(xtemp, ytemp, 0);
		map_.drawUnderTiles();
	glPopMatrix();

	timeProfiler.exit("under tiles");
	timeProfiler.exit("map");

	timeProfiler.enter("lvz");
	if(lvzManager_)
	{
		//lvzManager_->drawMapObjects();
		lvzManager_->drawMapObjects(xstart*16, ystart*16, xend*16, yend*16, LVZ_LAYER_AfterBackground);
		lvzManager_->drawMapObjects(xstart*16, ystart*16, xend*16, yend*16, LVZ_LAYER_AfterChat);
		lvzManager_->drawMapObjects(xstart*16, ystart*16, xend*16, yend*16, LVZ_LAYER_AfterGauges);
		lvzManager_->drawMapObjects(xstart*16, ystart*16, xend*16, yend*16, LVZ_LAYER_AfterShips);
		lvzManager_->drawMapObjects(xstart*16, ystart*16, xend*16, yend*16, LVZ_LAYER_AfterTiles);
		lvzManager_->drawMapObjects(xstart*16, ystart*16, xend*16, yend*16, LVZ_LAYER_AfterWeapons);
		lvzManager_->drawMapObjects(xstart*16, ystart*16, xend*16, yend*16, LVZ_LAYER_BelowAll);
		lvzManager_->drawMapObjects(xstart*16, ystart*16, xend*16, yend*16, LVZ_LAYER_TopMost);
	}
	timeProfiler.exit("lvz");

GLUtil::exitOrthoMode();
}

void SubspaceZone::drawRadarItems(const SubspaceRadar& radar) const
{
	//draw prizes


	//draw balls
	//radar->drawRadarBall();
	const SubspacePlayer* myPlayer = 0;
	if(players_.find(getMyPlayerID()) != players_.end())
		myPlayer = (*players_.find(getMyPlayerID())).second;

	//draw bomb explosions
	EffectList::const_iterator e;
	for(e = effects_.begin(); e != effects_.end(); ++e)
	{
		if(dynamic_cast<SubspaceExplosion*>(*e) != 0)			//TODO: find a better way to check bomb explosions
			radar.drawRadarExplosion((*e)->getPosition());
	}

	//draw flags
	FlagMap::const_iterator f;
	for(f = flags_.begin(); f != flags_.end(); ++f)
	{
		if((*f).second && myPlayer)
		{
			if((*f).second->getTeam() == myPlayer->getTeam())
				radar.drawRadarFlag((*f).second->getPosition());
		}
	}

	//draw bombs and mines
	if(!myPlayer)
		return;

	const ShipSettings& settings = arenaSettings_.ships[myPlayer->getShipType()];
	
	BombList::const_iterator b;
	for(b = bombs_.begin(); b != bombs_.end(); ++b)
	{
		if((*b)->isMine())
		{
			if(settings.SeeMines)
				radar.drawRadarMine((*b)->getPosition());
		}
		else if(settings.SeeBombLevel <= (Uint)(*b)->getLevel())
		{
			radar.drawRadarMine((*b)->getPosition());
		}
	}	

	//draw players
	SubspacePlayerMap::const_iterator p;
	for(p = players_.begin(); p != players_.end(); ++p)
	{
		if((*p).second == myPlayer)
			radar.drawRadarMyPlayer((*p).second);
		else
			radar.drawRadarPlayer((*p).second);
	}
}

template <class ListType>
void SubspaceZone::drawList(const ListType& list) const
{
	ListType::const_iterator i;
	ListType::value_type item;

	for(i = list.begin(); i != list.end(); ++i)
	{
		item = (*i);
		if(item)
		{
			item->display();
		}
	}
}

template <class MapType>
void SubspaceZone::drawMap(const MapType& map) const
{
	MapType::const_iterator i;
	MapType::mapped_type item;

	for(i = map.begin(); i != map.end(); ++i)
	{
		item = (*i).second;
		if(item)
		{
			item->display();
		}
	}
}



void positionThreadRoutine(void* ssZone)
{
	SubspaceZone* zone = static_cast<SubspaceZone*>(ssZone);
	Uint32 sleepDelay;

	printf("Started position routine.\n");
	while(zone->connected_)
	{
		zone->sendPosition();
		
		if(zone->getArenaSettings().SendPositionDelay)
            sleepDelay = zone->getArenaSettings().SendPositionDelay*4;	//4x just because thats what the server expects
		else
			sleepDelay = 100;

		Sleep(sleepDelay);
	}
	printf("Stopped position routine.");

	ExitThread(0);	
}