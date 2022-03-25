#include "SubspaceZone.h"

#include "zlib.h"

#include "Checksum.h"
#include "SubspacePlayer.h"
#include "SubspaceUtil.h"

//0x01
void SubspaceZone::doSetMyPlayerID(Uint16 id)
{
	myPlayerID_ = id;

	initArena();	//TODO: is this the correct place to init the arena?
}

//0x02
void SubspaceZone::doInGame()
{
	if(mapFilename_.size() > 0)
	{
		loadMap(zonePath_ + mapFilename_);
	}

	connected_ = true;
	
	positionThread_.start(this);		//TODO: move to player adapter?

	onEnterArena();
}

//0x03
void SubspaceZone::doPlayerEntering(PlayerData& p)
{
	SubspacePlayer* newPlayer = new SubspacePlayer(p);
	
	newPlayer->setSubspaceVelocity(Vector(0, 0, 0));
	newPlayer->setSubspaceDirection(0);

	addPlayer(newPlayer, p.playerID);	
}

//0x04
void SubspaceZone::doPlayerLeaving(Uint16 id)
{
	removePlayer(id);
}

//0x05													
void SubspaceZone::doPositionWeapon(
	Uint16 playerID, Uint16 timer, Sint16 posX, Sint16 posY, Sint16 velX, Sint16 velY, 
	Uint8 dir, PlayerStates stateInfo, PlayerItems itemInfo, WeaponData weaponInfo, 
	Uint16 bounty, Uint16 energy, Uint16 timestamp, Uint8 checksum, Uint8 ping, Uint16 s2cLag)
{
	SubspacePlayer* player = getPlayer(playerID);
	
	if(!player)
	{
		//this->debugLog("Position Update, unknown player: %d", playerID);
		return;
	}

	/***********************************************
	 * Check direction with the discrete 40 angles *
	 ***********************************************/

	player->setSubspaceDirection(dir);
	player->setSubspacePosition(Vector(posX, posY, 0));
	player->setSubspaceVelocity(Vector(velX, velY, 0));

	PlayerData p;

	//p.bounty = bounty;
	//p.energy = energy;
	//p.items = itemInfo;
	//p.ping = ping;
	//p.s2cLag = s2cLag;
	//p.state.u = stateInfo.u;
	//p.items = player->getItemSettings();
				
	player->setPlayerData(p);
			
	player->die(false);
	//player->setPlayerData(*p);
	//player->fireWeapon(weaponInfo, zone_->getArenaSettings());
	player->fireWeapon(weaponInfo);
}

//0x06
void SubspaceZone::doPlayerDeath(Uint16 killerID, Uint16 killedID, Uint16 bounty)
{
	SubspacePlayer* killer = 0;
	SubspacePlayer* killed = 0;

	killer = getPlayer(killerID);
	killed = getPlayer(killedID);

	if(killed)
		playerDeath(killedID);

	if(killer)
		killer->killedPlayer(killed->getId(), bounty);
		
	/*if(killer && killed)
		addStatusMessage(killed->getName() + "(" + AsciiUtil::itoa(bounty) + ") killed by: " + killer->getName(), COLOR_Yellow);*/
}

//0x07
/*void SubspaceZone::doChat(Uint16 playerID, string message, Uint8 chatType, Uint8 soundByte)
{
	const SubspacePlayer* player = getPlayer(playerID);

	ChatMessage newMessage;
	int addId = 0;
	if(player)
		newMessage = ChatMessage(player->getName(), message, (ChatType)chatType);
	else
		newMessage = ChatMessage("", message, (ChatType)chatType);

	//chat_->writeMessage(newMessage.sender, newMessage.getNameColor(), newMessage.text + "\n", newMessage.getTextColor());
}*/

//0x08											
void SubspaceZone::doPlayerPrize(Uint16 playerID, Uint16 xTile, Uint16 yTile, Uint16 prizeType, Uint32 timestamp)
{
	playerGotPrize(playerID, (PrizeType)prizeType, xTile, yTile);
}

//0x09	
void SubspaceZone::doScoreUpdate(Uint16 playerID, Uint32 killPoints, Uint32 flagPoints, Uint16 wins, Uint16 losses)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
	{
		printf("Unknown player [%d] score update\n", playerID);
		return;
	}
	player->setScore(killPoints, flagPoints, wins, losses);		
}

//0x0A											
void SubspaceZone::doPasswordResponse(BYTE response, const string& message, Uint32 version, BYTE regForm, Uint32 newsChecksum)
{
	printf("Login response-\n");
	printf("\t%d: %s\n", response, message.c_str());
	printf("\tversion: %d\n", version);
	printf("\tregform: %d\n", regForm);
	printf("\tnews checksum: %d\n", newsChecksum);

	if(regForm == 1) //wants a reg form
	{
		sendRegForm();
	}
	
	if(response == 1) //Unknown user
	{
		// TODO: implement login adapter?
		 sendPassword(username_, password_, true);		//TODO: move this into the network?
	}
	else
	{
		connected_ = true;
		onEnterZone();
		sendArenaLogin(startArena_);	//TODO: move to login adapter?
	}
}

//0x0D
void SubspaceZone::doPlayerChangeTeam(Uint16 playerID, Uint16 team, Uint8 ship)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;

	Uint16 oldTeam = player->getTeam();
	Uint8 oldShip = player->getShipType();

	if(ship & 0x80)	 //dont change ship if high byte is set
	{
		playerChangeTeam(playerID, team);		
	}
	else
	{
		playerChangeTeam(playerID, team);
		playerChangeShip(playerID, (ShipType)ship);		
	}
}

//0x0E
void SubspaceZone::doTurretLink(Uint16 requesterID, Uint16 destID)
{
	createTurret(requesterID, destID);
}

//0x0F
void SubspaceZone::doSetArenaSettings(const ArenaSettings& settings)
{
	//setArenaSettings(settings);
	arenaSettings_ = settings;

	initPrizes();
}

//0x12
void SubspaceZone::doFlagPosition(Uint16 flagID, Uint16 xTile, Uint16 yTile, Uint16 teamID)
{
	//TODO: add subspace util function for tile->actual coords
	//Vector pos((double)xTile*SubspaceMap::tileWidth, (double)yTile*SubspaceMap::tileHeight, 0);
	Vector pos;
	tileToObjectCoords(xTile, yTile, &pos);
		
	SubspaceGameObject::subspacePositionToStandardPosition(pos);

	setFlag(flagID, teamID, pos);	
}

//0x13
void SubspaceZone::doFlagClaim(Uint16 flagID, Uint16 playerID)
{
	playerGotFlag(playerID, flagID);	
}

//0x15
void SubspaceZone::doDestroyTurretLink(Uint16 playerID)
{
	destroyTurret(playerID);
}

//0x16
void SubspaceZone::doFlagDrop(Uint16 playerID)
{
	playerDropFlag(playerID);
}

//0x18
void SubspaceZone::doServerSyncRequest(Uint32 prizeSeed, Uint32 doorSeed, Uint32 timestamp, Uint32 checksumKey)
{
	// setServerTimestamp(timestamp);
	serverTime_ = timestamp;

	// setPrizeSeed(prizeSeed);
	// setDoorSeed(doorSeed);
	prizeSeed_ = prizeSeed;
	doorSeed_ = doorSeed;
	
	sendChecksum(checksumKey);
}

//0x1D
void SubspaceZone::doPlayerChangeShip(Uint16 playerID, Uint16 team, Uint8 ship)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;

	playerChangeShip(playerID, (ShipType)ship);
	playerChangeTeam(playerID, team);
}

//0x1F
void SubspaceZone::doPlayerBanner(Uint16 playerID, const string& bannerData)
{
	SubspacePlayer* player = getPlayer(playerID);
	if(!player)
		return;

	SubspaceBanner banner;
	banner.loadBanner(bannerData.c_str());
	player->setBanner(banner);	
}

//0x21
void SubspaceZone::doBrickPlace(vector<BrickData> bricks)
{
	vector<BrickData>::iterator i;
	for(i = bricks.begin(); i != bricks.end(); ++i)
	{
		BrickData& brick = (*i);
		//TODO: add timestamp use
		Uint16 newY1= SubspaceMap::maxTileY - brick.y1 - 1;
		Uint16 newY2 = SubspaceMap::maxTileY - brick.y2 - 1;
		dropBrick(brick.team, brick.x1, newY2, brick.x2, newY1, brick.timestamp);
	}
}

//0x27
void SubspaceZone::doKeepAlive()
{
	//this->debugLog("Keep Alive");
}

//0x28
void SubspaceZone::doPosition(											
	Uint8 playerID, Uint8 bounty, Sint16 posX, Sint16 posY, Sint16 velX, Sint16 velY, Uint8 dir, 
	PlayerStates states, PlayerItems items, Uint16 energy, Uint16 timestamp, Uint8 ping, 
	Uint16 s2cLag, Uint16 timer)
{
	if(states.flash)
	{
		debugout << "FLASHING" << endl;
		//printf("FLASHING\n");
	}

	PlayerData p;
	SubspacePlayer* player = getPlayer(playerID);

	if(!player)
		return;

	player->setSubspaceDirection(dir);
	player->setSubspacePosition(Vector(posX, posY, 0));
	player->setSubspaceVelocity(Vector(velX, velY, 0));

	//p.bounty = bounty;
	//p.energy = energy;
	//p.items = itemInfo;
	//p.ping = ping;
	//p.s2cLag = s2cLag;
	//p.state.u = stateInfo.u;

	player->setPlayerData(p);

	player->die(false);
}
								
//0x29
void SubspaceZone::doMapInfo(const string& mapFilename, Uint32 remoteFileChecksum, Uint32 downloadSize)
{
	Checksum checksum;
	string levelFilename = zonePath_ + mapFilename;

	//TODO: move to File adapter?

	Uint32 localFileChecksum = checksum.fileChecksum(levelFilename.c_str());
	
	if(localFileChecksum != remoteFileChecksum)
	{
		sendMapRequest();
	}
	else
	{
		printf("Map check successful: %s\n", mapFilename.c_str());

		mapFilename_ = mapFilename;
		validMap_ = true;
	}
}

//0x2A	//always a compressed map file
void SubspaceZone::doMapFile(const string& mapName, const string& data)
{
	// TODO: move this out to file adapter?

	//uncompress map (zip)
	unsigned long length = static_cast<unsigned long>(data.size());
	unsigned char* outBuffer = 0;
	unsigned long outSize = 0;
	
	unsigned char* dataBuffer = new unsigned char[data.size()];
	unsigned long dataSize = data.size();
	//memcpy(dataBuffer, data.c_str(), data.size());
	for(unsigned long i=0; i < data.size(); ++i)
		dataBuffer[i] = data[i];

	if(!uncompressBuffer(dataBuffer, dataSize, &outBuffer, &outSize))
	{
		printf("Failed to decompress map file \"%s\".\n", mapName.c_str());
		//corrupt file
		return;
	}

	if(!saveBuffer((zonePath_+mapName).c_str(), outBuffer, outSize))
	{
		printf("Failed to save map file \"%s\".\n", (zonePath_+mapName).c_str());
		//cant save
		return;
	}

	mapFilename_ = mapName;
	loadMap(zonePath_ + mapFilename_);
}

//0x2B
void SubspaceZone::doKothTimerSet(Uint32 time)
{
	SubspacePlayer* player = getMyPlayer();

	if(!player)
		return;
	
	player->setKothTime(time * 10.0);	//time conversion
}

//0x2C
void SubspaceZone::doKothReset(Uint32 time, Uint16 playerID)
{
	SubspacePlayer* player = getPlayer(playerID);
	
	if(!player)
		return;

	player->setKothTime(time * 10.0);		//TODO: fix this
}

//0x2E
void SubspaceZone::doBallPosition(Uint8 ballID, Uint16 xPixel, Uint16 yPixel, Sint16 xVelocity, 
	Sint16 yVelocity, Uint16 ownerID, Uint16 timestamp)
{
	Vector pos(xPixel, yPixel, 0);
	SubspaceGameObject::subspacePositionToStandardPosition(pos);

	Vector vel(xVelocity, yVelocity, 0);
	SubspaceGameObject::subspaceVelocityToStandardVelocity(vel);

	setBall(ballID, ownerID, pos, vel, timestamp);	
}

//0x2F
/*void SubspaceZone::doArenaList(vector<string> names, vector<Uint> population)
{
	vector<string>::iterator i;
	vector<Uint>::iterator j;
	for(i = names.begin(), j = population.begin(); i != names.end(); ++i, ++j)
	{
		//this->log("Arena \"%s\" [%d]\n", (*i).c_str(), (*j));
	}
}*/

//0x31
void SubspaceZone::doPastLogin()
{
	//this->debugLog("Past login");
}