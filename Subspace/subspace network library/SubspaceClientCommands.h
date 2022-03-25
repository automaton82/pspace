#ifndef _SUBSPACECLIENTCOMMANDS_H_
#define _SUBSPACECLIENTCOMMANDS_H_

#include "DataTypes.h"

#include "SubspacePacketInterpreter.h"
using namespace SubspacePacketInterpreter;

#include "WeaponSettings.h"

class SubspaceClientCommandListener;


// commands that are received/executed on the client, sent by the server
class SubspaceClientCommand
{
public:
	virtual ~SubspaceClientCommand();

	virtual void execute(SubspaceClientCommandListener* commandListener) = 0;
};

namespace SubspaceClientCommands
{
//0x01
	struct SetMyPlayerID : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);
	
		Uint16 id;
	};

//0x02
	struct InGame : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);
	};

//0x03
	struct PlayerEntering : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);
	
		PlayerData p;
	};

//0x04
	struct PlayerLeaving : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint16 id;
	};
		
//0x05													
	struct PositionWeapon : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint16 playerID; 
		Uint16 timer; 
		Sint16 posX, posY, velX, velY;
		Uint8 dir; 
		PlayerStates states; 
		PlayerItems items; 
		WeaponData weapon; 
		Uint16 bounty, energy;
        Uint16 timestamp; 
		Uint8 checksum; 
		Uint8 ping; 
		Uint16 s2cLag;
	};

//0x06
	struct PlayerDeath : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint16 killerID, killedID, bounty;
	};

/*
//0x07
	struct Chat : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint16 playerID;
		string message;
		Uint8 chatType;
		Uint8 soundByte;
	};
*/

//0x08											
	struct PlayerPrize : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint16 playerID;
		Uint16 xTile, yTile;
		Uint16 prizeType;
		Uint32 timestamp;
	};

//0x09
	struct ScoreUpdate : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint16 playerID;
		Uint32 killPoints, flagPoints;
		Uint16 wins, losses;		
	};

//0x0A		
	struct PasswordResponse : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		BYTE response;
		string message;
		Uint32 version;
		BYTE regForm;
		Uint32 newsChecksum;
	};

//0x0D
	struct PlayerChangeTeam : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint16 playerID;
		Uint16 team;
		Uint8 ship;
	};

//0x0E
	struct TurretLink : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint16 requesterID;
		Uint16 destID;
	};

//0x0F
	struct SetArenaSettings : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		ArenaSettings arenaSettings;
	};

//0x12
	struct FlagPosition : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint16 flagID;
		Uint16 xTile, yTile;
		Uint16 teamID;
	};

//0x13
	struct FlagClaim : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint16 flagID;
		Uint16 playerID;
	};

//0x15
	struct DestroyTurretLink : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint16 playerID;
	};

//0x16
	struct FlagDrop : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint16 playerID;
	};

//0x18
	struct ServerSyncRequest : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint32 prizeSeed, doorSeed, timestamp, checksumKey;
	};

//0x1D
	struct PlayerChangeShip : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint16 playerID;
		Uint16 team;
		Uint8 ship;
	};

//0x1F
	struct PlayerBanner : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint16 playerID;
		string bannerData;
	};

//0x21
	struct BrickPlace : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		vector<BrickData> bricks;
	};

//0x27
	struct KeepAlive : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);
	};

//0x28
	struct Position : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint8 playerID, bounty;
		Sint16 posX, posY, velX, velY;
		Uint8 dir;
		PlayerStates stateInfo;
		PlayerItems itemInfo;
		Uint16 energy;
		Uint16 timestamp;
		Uint8 ping;
		Uint16 s2cLag;
		Uint16 timer;
	};

//0x29
	struct MapInfo : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		string mapFilename;
		Uint32 remoteFileChecksum;
		Uint32 downloadSize;
	};

//0x2A	//always a compressed map file
	struct MapFile : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		string mapName;
		string data;
	};

//0x2B
	struct KothTimerSet : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint32 time;
	};

//0x2C
	struct KothReset : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint32 time;
		Uint16 playerID;
	};

//0x2E
	struct BallPosition : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		Uint8 ballID;
		Uint16 xPixel, yPixel;
		Sint16 xVelocity, yVelocity;
		Uint16 ownerID;
		Uint16 timestamp;
	};

//0x2F
	struct ArenaList : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);

		vector<string> names;
		vector<Uint> population;
	};

//0x31
	struct PastLogin : public SubspaceClientCommand
	{
		void execute(SubspaceClientCommandListener* listener);
	};
}

#endif