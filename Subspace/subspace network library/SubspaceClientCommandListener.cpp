#include "SubspaceClientCommandListener.h"

#include "SubspaceClientCommands.h"

/*
SubspaceClientCommandListener::SubspaceClientCommandListener()
{}

SubspaceClientCommandListener::~SubspaceClientCommandListener()
{}
*/

/*
void SubspaceClientCommandListener::handleCommand(SubspaceClientCommand* command)
{
	command->execute(this);
}
*/

//0x01
void SubspaceClientCommandListener::doSetMyPlayerID(Uint16 id)
{}
//0x02
void SubspaceClientCommandListener::doInGame()
{}
//0x03
void SubspaceClientCommandListener::doPlayerEntering(PlayerData& p)
{}
//0x04
void SubspaceClientCommandListener::doPlayerLeaving(Uint16 id)
{}
//0x05													
void SubspaceClientCommandListener::doPositionWeapon(
	Uint16 playerID, Uint16 timer, Sint16 posX, Sint16 posY, Sint16 velX, Sint16 velY, 
	Uint8 dir, PlayerStates stateInfo, PlayerItems itemInfo, WeaponData weaponInfo, 
	Uint16 bounty, Uint16 energy, Uint16 timestamp, Uint8 checksum, Uint8 ping, Uint16 s2cLag)
{}
//0x06
void SubspaceClientCommandListener::doPlayerDeath(Uint16 killerID, Uint16 killedID, Uint16 bounty)
{}
//0x07
void SubspaceClientCommandListener::doChat(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte)
{}
//0x08
void SubspaceClientCommandListener::doPlayerPrize(Uint16 playerID, Uint16 xTile, Uint16 yTile, Uint16 prizeType, Uint32 timestamp)
{}
//0x09
void SubspaceClientCommandListener::doScoreUpdate(Uint16 playerID, Uint32 killPoints, Uint32 flagPoints, Uint16 wins, Uint16 losses)
{}
//0x0A											
void SubspaceClientCommandListener::doPasswordResponse(BYTE response, const string& message, Uint32 version, BYTE regForm, Uint32 newsChecksum)
{}
//0x0D
void SubspaceClientCommandListener::doPlayerChangeTeam(Uint16 playerID, Uint16 team, Uint8 ship)
{}
//0x0E
void SubspaceClientCommandListener::doTurretLink(Uint16 requesterID, Uint16 destID)
{}
//0x0F
void SubspaceClientCommandListener::doSetArenaSettings(const ArenaSettings& settings)
{}
//0x12
void SubspaceClientCommandListener::doFlagPosition(Uint16 flagID, Uint16 xTile, Uint16 yTile, Uint16 teamID)
{}
//0x13
void SubspaceClientCommandListener::doFlagClaim(Uint16 flagID, Uint16 playerID)
{}
//0x15
void SubspaceClientCommandListener::doDestroyTurretLink(Uint16 playerID)
{}
//0x16
void SubspaceClientCommandListener::doFlagDrop(Uint16 playerID)
{}
//0x18
void SubspaceClientCommandListener::doServerSyncRequest(Uint32 prizeSeed, Uint32 doorSeed, Uint32 timestamp, Uint32 checksumKey)
{}
//0x1D
void SubspaceClientCommandListener::doPlayerChangeShip(Uint16 playerID, Uint16 team, Uint8 ship)
{}
//0x1F
void SubspaceClientCommandListener::doPlayerBanner(Uint16 playerID, const string& bannerData)
{}
//0x21
void SubspaceClientCommandListener::doBrickPlace(vector<BrickData> bricks)
{}
//0x27
void SubspaceClientCommandListener::doKeepAlive()
{}
//0x28
void SubspaceClientCommandListener::doPosition(											
	Uint8 playerID, Uint8 bounty, Sint16 posX, Sint16 posY, Sint16 velX, Sint16 velY, Uint8 dir, 
	PlayerStates states, PlayerItems items, Uint16 energy, Uint16 timestamp, Uint8 ping, 
	Uint16 s2cLag, Uint16 timer)
{}								
//0x29
void SubspaceClientCommandListener::doMapInfo(const string& mapFilename, Uint32 remoteFileChecksum, Uint32 downloadSize)
{}
//0x2A	//always a compressed map file
void SubspaceClientCommandListener::doMapFile(const string& mapName, const string& data)
{}
//0x2B
void SubspaceClientCommandListener::doKothTimerSet(Uint32 time)
{}
//0x2C
void SubspaceClientCommandListener::doKothReset(Uint32 time, Uint16 playerID)
{}
//0x2E
void SubspaceClientCommandListener::doBallPosition(			
	Uint8 ballID, Uint16 xPixel, Uint16 yPixel, Sint16 xVelocity, Sint16 yVelocity, Uint16 ownerID,
	Uint16 timestamp)
{}
//0x2F
void SubspaceClientCommandListener::doArenaList(vector<string> names, vector<Uint> population)
{}
//0x31
void SubspaceClientCommandListener::doPastLogin()
{}