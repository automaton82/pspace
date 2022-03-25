#include "SubspaceClientCommands.h"

#include "SubspaceClientCommandListener.h"


SubspaceClientCommand::~SubspaceClientCommand()
{
}

namespace SubspaceClientCommands
{

//0x01
void SetMyPlayerID::execute(SubspaceClientCommandListener* listener)
{
	listener->doSetMyPlayerID(id);
}

//0x02
void InGame::execute(SubspaceClientCommandListener* listener)
{
	listener->doInGame();
}

//0x03
void PlayerEntering::execute(SubspaceClientCommandListener* listener)
{
	listener->doPlayerEntering(p);
}

//0x04
void PlayerLeaving::execute(SubspaceClientCommandListener* listener)
{
	listener->doPlayerLeaving(id);
}
		
//0x05													
void PositionWeapon::execute(SubspaceClientCommandListener* listener)
{
	listener->doPositionWeapon(	playerID, timer, posX, posY, velX, velY, dir, 
		states, items, weapon, bounty, energy, timestamp, checksum, ping, s2cLag);
}

//0x06
void PlayerDeath::execute(SubspaceClientCommandListener* listener)
{
	listener->doPlayerDeath(killerID, killedID, bounty);
}
	
/*
//0x07
void Chat::execute(SubspaceClientCommandListener* listener)
{
	listener->doChat(playerID, message, chatType, soundByte);
}
*/

//0x08											
void PlayerPrize::execute(SubspaceClientCommandListener* listener)
{
	listener->doPlayerPrize(playerID, xTile, yTile, prizeType, timestamp);
}

//0x09
void ScoreUpdate::execute(SubspaceClientCommandListener* listener)
{
	listener->doScoreUpdate(playerID, killPoints, flagPoints, wins, losses);
}

//0x0A		
void PasswordResponse::execute(SubspaceClientCommandListener* listener)
{
	listener->doPasswordResponse(response, message, version, regForm, newsChecksum);
}

//0x0D
void PlayerChangeTeam::execute(SubspaceClientCommandListener* listener)
{
	listener->doPlayerChangeTeam(playerID, team, ship);	
}

//0x0E
void TurretLink::execute(SubspaceClientCommandListener* listener)
{
	listener->doTurretLink(requesterID, destID);
}

//0x0F
void SetArenaSettings::execute(SubspaceClientCommandListener* listener)
{
	listener->doSetArenaSettings(arenaSettings);
}

//0x12
void FlagPosition::execute(SubspaceClientCommandListener* listener)
{
	listener->doFlagPosition(flagID, xTile, yTile, teamID);
}

//0x13
void FlagClaim::execute(SubspaceClientCommandListener* listener)
{
	listener->doFlagClaim(flagID, playerID);
}

//0x15
void DestroyTurretLink::execute(SubspaceClientCommandListener* listener)
{
	listener->doDestroyTurretLink(playerID);
}

//0x16
void FlagDrop::execute(SubspaceClientCommandListener* listener)
{
	listener->doDestroyTurretLink(playerID);
}

//0x18
void ServerSyncRequest::execute(SubspaceClientCommandListener* listener)
{
	listener->doServerSyncRequest(prizeSeed, doorSeed, timestamp, checksumKey);
}

//0x1D
void PlayerChangeShip::execute(SubspaceClientCommandListener* listener)
{
	listener->doPlayerChangeShip(playerID, team, ship);
}

//0x1F
void PlayerBanner::execute(SubspaceClientCommandListener* listener)
{
	listener->doPlayerBanner(playerID, bannerData);
}

//0x21
void BrickPlace::execute(SubspaceClientCommandListener* listener)
{
	listener->doBrickPlace(bricks);
}

//0x27
void KeepAlive::execute(SubspaceClientCommandListener* listener)
{
	listener->doKeepAlive();
}

//0x28
void Position::execute(SubspaceClientCommandListener* listener)
{
	listener->doPosition(playerID, bounty, posX, posY, velX, velY, dir, stateInfo, itemInfo, energy, 
		timestamp, ping, s2cLag, timer);
}

//0x29
void MapInfo::execute(SubspaceClientCommandListener* listener)
{
	listener->doMapInfo(mapFilename, remoteFileChecksum, downloadSize);
}

//0x2A	//always a compressed map file
void MapFile::execute(SubspaceClientCommandListener* listener)
{
	listener->doMapFile(mapName, data);
}

//0x2B
void KothTimerSet::execute(SubspaceClientCommandListener* listener)
{
	listener->doKothTimerSet(time);
}

//0x2C
void KothReset::execute(SubspaceClientCommandListener* listener)
{
	listener->doKothReset(time, playerID);
}

//0x2E
void BallPosition::execute(SubspaceClientCommandListener* listener)
{
	listener->doBallPosition(ballID, xPixel, yPixel, xVelocity, yVelocity, ownerID, timestamp);
}

//0x2F
void ArenaList::execute(SubspaceClientCommandListener* listener)
{
	listener->doArenaList(names, population);
}

//0x31
void PastLogin::execute(SubspaceClientCommandListener* listener)
{
	listener->doPastLogin();
}

}
