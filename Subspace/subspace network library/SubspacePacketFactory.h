//David Millman

#include "DataTypes.h"
#include "SubspacePacket.h"
#include "windows.h"

#ifndef _SUBSPACEPACKETFACTORY_H_
#define _SUBSPACEPACKETFACTORY_H_

using std::string;

namespace SubspacePacketFactory
{
	/****************
	 * Core Packets *
	 ****************/

	//TODO: these should all be functions

	//0x0001
	SubspacePacket encryptionRequest(Uint32 encryptionKey);

	//0x0003
	SubspacePacket reliableHeader(Uint32 packetID);

	//0x0004
	SubspacePacket reliableAck(Uint32 packetID);

	//0x0005
	SubspacePacket syncRequest(Uint32 packetsent, Uint32 packetsReceived, Uint32 timeStamp);

	//0x0006
	SubspacePacket syncResponse(Uint32 serverTimeStamp, Uint32 clientTimeStamp);

	//0x0007
	SubspacePacket disconnect();

	//0x0008
	SubspacePacket smallChunk(const string& data);

	//0x0009
	SubspacePacket smallChunkTail(const string& data);

	/****************************
	 * Client To Server Packets *
	 ****************************/

	//0x01
	SubspacePacket arenaLogin(Uint8 shipType, Uint16 allowAudio, Uint16 resX, Uint16 resY, Uint16 arenaType, string arenaName = "");

	//0x02
	SubspacePacket arenaLeave();						

	//0x03
	SubspacePacket position(
		Uint16 xPosition, Uint16 xVelocity, Uint16 yPosition, Uint16 yVelocity, Uint8 direction, 
		Uint16 energy, Uint16 bounty, Uint8 togglables, Uint16 weaponInfo, Uint32 itemInfo,
		Uint16 latency, Uint16 timer, Uint32 timestamp);

	//0x05
	SubspacePacket death(Uint16 killerID, Uint16 bounty);
		
	//0x06
	SubspacePacket chat(Uint8 chatType, Uint8 soundByte, string text, Uint16 playerID = 0);

	//0x08
	SubspacePacket spectate(Uint16 playerID);
	
	//0x09
	SubspacePacket password(bool newUser, string username, string password);

	//0x0B
	SubspacePacket SubspaceExeUpdateRequest();

	//0x0C
	SubspacePacket mapRequest();

	//0x0D
	SubspacePacket newsRequest();

	//0x0E
	SubspacePacket voiceMeSubspaceage(Uint32 wavfileSize, Uint8* wavFile);

	//0x0F
	SubspacePacket changeTeam(Uint16 newTeam);

	//0x10
	SubspacePacket attachRequest(Uint16 playerID);
	
	//0x13
	SubspacePacket flagRequest(Uint16 flagID);

	//0x15
	SubspacePacket flagsDrop();

	//0x16
	SubspacePacket fileTransfer(string filename, Uint8* zlibCompressedFile);

	//0x17
	SubspacePacket regFormResponse(string realName, string email, string city, string state,
		Uint8 gender, Uint8 age, bool connectFromHome, bool connectFromWork, bool connectFromSchool,
		Uint32 processorType, string windowsRegRealName, string windowsRegOrganization,
		string displayDesc, string monitorDesc, string modem1Desc, string modem2Desc, string mouseDesc, 
		string net00Desc, string net01Desc, string printerDesc, 
		string media0Desc, string media1Desc, string media2Desc, string media3Desc, string media4Desc);

	//0x18
	SubspacePacket setShipType(Uint8 shipType);

	//0x19
	SubspacePacket setBanner(Uint8 bannerData[96]);

	//0x1A
	SubspacePacket securityChecksum(Uint32 weaponCount, Uint32 settingsChecksum, Uint32 subspaceEXEChecksum,
		Uint32 mapChecksum, Uint32 s2cSlowTotal, Uint32 s2cFastTotal, Uint16 s2cSlowCurrent,
		Uint16 s2cFastCurrent, Uint16 s2cRelOut, Uint16 ping, Uint16 pingAve, Uint16 pingLow, Uint16 pingHigh,
		bool slowFrameRate);

	//0x1B
	SubspacePacket securityViolation(Uint8 violationID);

	//0x1C
	SubspacePacket brickDrop(Uint16 xTile, Uint16 yTile);

	//0x1E
	SubspacePacket kothTimerEnded();

	//0x1F
	SubspacePacket fireBall(Uint8 ballID, Uint16 xPixels, Uint16 xVelocity, 
		Uint16 yPixels, Uint16 yVelocity, Uint16 playerID, Uint32 timestamp);

	//0x20
	SubspacePacket ballRequest(Uint8 ballID, Uint32 timestamp);
};

#endif