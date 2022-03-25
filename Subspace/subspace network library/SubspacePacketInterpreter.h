//David Millman

#ifndef _SUBSPACEPACKETINTERPRETER_H_
#define _SUBSPACEPACKETINTERPRETER_H_

#include "windows.h"

#include <vector>
using std::vector;

#include "DataTypes.h"
#include "SubspacePacket.h"
#include "ArenaSettings.h"
#include "PlayerSettings.h"

namespace SubspacePacketInterpreter
{
	/****************
	 * Core Packets *
	 ****************/

	//TODO: these should all be functions

	//0x0002
	void encryptionResponse(const SubspacePacket& p, Uint32* encKey);

	//0x0003
	void reliablePacket(const SubspacePacket& p, Uint32* ackID, SubspacePacket* body);

	//0x0005
	void syncRequest(const SubspacePacket& p, Uint32* timeStamp, Uint32* packetsSent, Uint32* packetsReceived);

	//0x0006
	void syncResponse(const SubspacePacket& p, Uint32* timestampAtRequester, Uint32* timestampAtRequestee);

	//0x0008
	void smallChunk(const SubspacePacket& p, SubspacePacket* data);

	//0x0009
	void smallChunkTail(const SubspacePacket& p, SubspacePacket* data);

	//0x000A
	void fileTransfer(const SubspacePacket& p, Uint32* fileSize, string* data);

	/****************************
	 * Server To Client Packets *
	 ****************************/

	//0x01
	void playerID(const SubspacePacket& p, Uint16* playerID);

	//0x02
	void inGame(const SubspacePacket& p);

	//0x03
	void playerEntering(const SubspacePacket& p, PlayerData* player, SubspacePacket* remainingPlayers);

	//0x04
	void playerLeaving(const SubspacePacket& p, Uint16* playerID);
	
	//0x05
	void positionWeapon(const SubspacePacket& p, Uint16* playerID, Uint16* bounty, 
		Uint16* xPosition, Uint16* xVelocity, Uint16* yPosition, Uint16* yVelocity, Uint8* direction, 
		Uint8* togglables, Uint16* weaponInfo, Uint32* itemInfo, Uint16* energy, 	
		Uint16* timestamp, Uint8* checksum, Uint8* ping, Uint16* s2cLag, Uint16* timer);

	//0x06
	void playerDeath(const SubspacePacket& p, Uint16* killerID, Uint16* playerKilledID, Uint16* bounty);

	//0x07
	void chat(const SubspacePacket& p, Uint8* chatType, Uint8* soundByte, Uint16* originatorID, string* message);

	//0x08
	void playerPrize(const SubspacePacket& p, Uint16* playerID, Uint16* xTiles, Uint16* yTiles, Uint16* prizeType, Uint32* timestamp);

	//0x09
	void scoreUpdate(const SubspacePacket& p, Uint16* playerID, Uint32* killPoints, Uint32* flagPoints, Uint16* wins, Uint16* losses);

	//0x0A
	void passwordResponse(const SubspacePacket &p, Uint8* loginResponse, string* message, Uint32* serverVersion, Byte* regForm, Uint32* newsChecksum);

	//0x0B
	void soccerGoal(const SubspacePacket& p, Uint16* scoringTeam, Uint32* teamPoints);

	//0x0C
	void playerVoice(const SubspacePacket& p, Uint16* playerID, Uint8* wavFileData);
	
	//0x0D
	void playerChangeTeam(const SubspacePacket& p, Uint16* playerID, Uint16* team, Uint8* ship);

	//0x0E
	void turretLink(const SubspacePacket& p, Uint16* requesterID, Uint16* destID);

	//0x0F
	void arenaSettings(const SubspacePacket& p, ArenaSettings* arena);

	//0x12
	void flagPosition(const SubspacePacket& p, Uint16* flagID, Uint16* xTiles, Uint16* yTiles, Uint16* team);

	//0x13
	void flagClaim(const SubspacePacket& p, Uint16* flagID, Uint16* playerID);

	//0x14
	void flagVictory(const SubspacePacket& p, Uint16* team, Uint32* pointsReceived);

	//0x15
	void destroyTurretLink(const SubspacePacket& p, Uint16* playerID);

	//0x16
	void flagDrop(const SubspacePacket& p, Uint16* playerID);

	//0x18
	void serverSyncRequest(const SubspacePacket& p, Uint32* prizeSeed, Uint32* doorSeed, Uint32* timestamp, Uint32* checksumKey);
	
	//0x19
	void requestFile(const SubspacePacket& p, string* localFileName, string* remoteFileName);

	//0x1C
	void playerChangeSpectate(const SubspacePacket& p);

	//0x1D
	void playerChangeShip(const SubspacePacket& p, Uint8* ship, Uint16* playerID, Uint16* team);

	//0x1F
	void playerBanner(const SubspacePacket& p, Uint16* playerID, string* bannerData);

	struct BrickData
	{
		Uint16 x1, y1, x2, y2;
		Uint16 team;
		Uint16 brickId;
		Uint32 timestamp;
	};

	//0x21
	void brickDrop(const SubspacePacket& p, vector<BrickData>& bricks);

	//0x27
	void keepAlive(const SubspacePacket& p);

	//0x28
	void position(const SubspacePacket& p, Uint8* playerID, Uint8* bounty, 
		Uint16* xPosition, Uint16* xVelocity, Uint16* yPosition, Uint16* yVelocity, Uint8* direction, 
		Uint8* togglables, Uint32* itemInfo, Uint16* energy, 
		Uint16* timestamp, Uint8* ping, Uint16* s2cLag, Uint16* timer);

	//0x29	
	void mapInfo(const SubspacePacket& p, string* name, Uint32* mapChecksum, Uint32* downloadSize);

	//0x2A
	void mapFile(const SubspacePacket& p, string* mapName, string* mapData);

	//0x2B
	void kothTimerSet(const SubspacePacket& p, Uint32* time);

	//0x2C
	void kothReset(const SubspacePacket& p, Uint8* unknown, Uint32* time, Uint16* playerID);

	//0x2E
	void ballPosition(const SubspacePacket& p, Uint8* ballID, Uint16* xPixel, 
		Uint16* yPixel, Sint16* xVelocity, Sint16* yVelocity, Uint16* ownerID, Uint16* timestamp);

	//0x2F
	void arenaList(const SubspacePacket& p, vector<string>& names, vector<Uint>& population);

	//0x31
	//void pastLogin(const SubspacePacket& p);
};

#endif