#ifndef _SUBSPACECLIENTCOMMANDLISTENER_H_
#define _SUBSPACECLIENTCOMMANDLISTENER_H_

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "DataTypes.h"
#include "CommandListenerBase.h"

#include "ArenaSettings.h"
#include "PlayerSettings.h"
#include "WeaponSettings.h"

#include "SubspacePacketInterpreter.h"		//For BrickData struct
using namespace SubspacePacketInterpreter;				

//class SubspaceClientCommand;
#include "SubspaceClientCommands.h"

class SubspaceClientCommandListener : 
	public CommandListenerBase<SubspaceClientCommandListener, SubspaceClientCommand>
{
public:

	//SubspaceClientCommandListener();
	//~SubspaceClientCommandListener();

	//virtual void handleCommand(SubspaceClientCommand* command);

	///////////////////////////////////////////////////////////////////////////
	// These functions are all no-ops, need to be implemented by subclass

	//0x01
	virtual void doSetMyPlayerID(Uint16 id);

	//0x02
	virtual void doInGame();	

	//0x03
	virtual void doPlayerEntering(PlayerData& p);

	//0x04
	virtual void doPlayerLeaving(Uint16 id);

	//0x05													
	virtual void doPositionWeapon(
		Uint16 playerID, Uint16 timer, Sint16 posX, Sint16 posY, Sint16 velX, Sint16 velY, 
		Uint8 dir, PlayerStates states, PlayerItems items, WeaponData weaponData, 
		Uint16 bounty, Uint16 energy, Uint16 timestamp, Uint8 checksum, Uint8 ping, Uint16 s2cLag);

	//0x06
	virtual void doPlayerDeath(Uint16 killerID, Uint16 killedID, Uint16 bounty);

	//0x07
	virtual void doChat(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte);

	//0x08											
	virtual void doPlayerPrize(Uint16 playerID, Uint16 xTile, Uint16 yTile, Uint16 prizeType, Uint32 timestamp);

	//0x09
	virtual void doScoreUpdate(Uint16 playerID, Uint32 killPoints, Uint32 flagPoints, Uint16 wins, Uint16 losses);

	//0x0A											
	virtual void doPasswordResponse(BYTE response, const string& message, Uint32 version, BYTE regForm, Uint32 newsChecksum);

	//0x0D
	virtual void doPlayerChangeTeam(Uint16 playerID, Uint16 team, Uint8 ship);	

	//0x0E
	virtual void doTurretLink(Uint16 requesterID, Uint16 destID);

	//0x0F
	virtual void doSetArenaSettings(const ArenaSettings& settings);

	//0x12
	virtual void doFlagPosition(Uint16 flagID, Uint16 xTile, Uint16 yTile, Uint16 teamID);

	//0x13
	virtual void doFlagClaim(Uint16 flagID, Uint16 playerID);

	//0x15
	virtual void doDestroyTurretLink(Uint16 playerID);

	//0x16
	virtual void doFlagDrop(Uint16 playerID);

	//0x18
	virtual void doServerSyncRequest(Uint32 prizeSeed, Uint32 doorSeed, Uint32 timestamp, Uint32 checksumKey);

	//0x1D
	virtual void doPlayerChangeShip(Uint16 playerID, Uint16 team, Uint8 ship);

	//0x1F
	virtual void doPlayerBanner(Uint16 playerID, const string& bannerData);

	//0x21
	virtual void doBrickPlace(vector<SubspacePacketInterpreter::BrickData> bricks);	

	//0x27
	virtual void doKeepAlive();				

	//0x28
	virtual void doPosition(											
		Uint8 playerID, Uint8 bounty, Sint16 posX, Sint16 posY, Sint16 velX, Sint16 velY, Uint8 dir, 
		PlayerStates stateInfo, PlayerItems itemInfo, Uint16 energy, Uint16 timestamp, Uint8 ping, 
		Uint16 s2cLag, Uint16 timer);
								
	//0x29
	virtual void doMapInfo(const string& mapFilename, Uint32 remoteFileChecksum, Uint32 downloadSize);

	//0x2A	//always a compressed map file
	virtual void doMapFile(const string& mapName, const string& data);

	//0x2B
	virtual void doKothTimerSet(Uint32 time);

	//0x2C
	virtual void doKothReset(Uint32 time, Uint16 playerID);

	//0x2E
	virtual void doBallPosition(			
		Uint8 ballID, Uint16 xPixel, Uint16 yPixel, Sint16 xVelocity, Sint16 yVelocity, Uint16 ownerID,
		Uint16 timestamp);

	//0x2F
	virtual void doArenaList(vector<string> names, vector<Uint> population);

	//0x31
	virtual void doPastLogin();
};

#endif