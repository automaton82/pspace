#ifndef _SUBSPACESERVERCOMMANDS_H_
#define _SUBSPACESERVERCOMMANDS_H_

#include <string>
using std::string;

#include "DataTypes.h"

#include "ChatSettings.h"
#include "PlayerSettings.h"

class SubspaceServerCommandListener;

// commands that are received/executed on the server, sent by the client
class SubspaceServerCommand
{
public:
	virtual ~SubspaceServerCommand();

	virtual void execute(SubspaceServerCommandListener* commandListener) = 0;
};

namespace SubspaceServerCommands
{
//0x01
	struct ArenaLogin : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);
	
		//Uint16 arenaID;
		string arenaName;
		Uint8 startShip;
		Uint resX, resY;
	};

//0x02
	struct ArenaLeave : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);		
	};

//0x03
	struct Position : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);		

		Uint16 xPosition, xVelocity, yPosition, yVelocity;
		Uint8 direction;
		Uint16 energy, bounty;
		Uint8 togglables;
		Uint16 weaponData;
		Uint32 items;
		Uint16 latency;
		Uint16 timer;
		Uint32 timestamp;
	};

//0x05
	struct Death : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);		

		Uint16 killerID;
		Uint16 bounty;
	};
/*
//0x06
	struct Chat : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);		

		Uint16 playerID;
		string message;
		ChatType type;
		Uint8 soundByte;
	};
*/

//0x0A
	struct Password : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);		

		string user;
		string password;
		bool newUser;
	};

//0x0C
	struct MapRequest : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);				
	};
	
//0x0F
	struct TeamChange : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);

		Uint16 teamID;
	};

//0x10
	struct AttachRequest : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);		

		Uint16 playerID;
	};

//0x13
	struct FlagRequest : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);		

		Uint16 flagID;
	};
	
//0x15
	struct FlagsDrop : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);		
	};
	
//0x17
	struct RegistrationForm : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);		
	};
	
//0x18
	struct ShipChange : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);

		ShipType ship;
	};
	
//0x1A
	struct ChecksumData : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);

		Uint32 arenaSettingsChecksum;
		Uint32 mapChecksum;
		Uint32 exeChecksum;
	};

//0x1E
	struct KothTimerEnded : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);
	};

//0x21
	struct BrickDrop : public SubspaceServerCommand
	{
		void execute(SubspaceServerCommandListener* listener);

		Uint16 xTile, yTile;
	};
}

#endif