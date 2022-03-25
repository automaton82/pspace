#ifndef _SUBSPACESERVERCOMMANDLISTENER_H_
#define _SUBSPACESERVERCOMMANDLISTENER_H_

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "DataTypes.h"
#include "CommandListenerBase.h"

#include "ArenaSettings.h"
#include "ChatSettings.h"
#include "PlayerSettings.h"

//class SubspaceServerCommand;
#include "SubspaceServerCommands.h"

class SubspaceServerCommandListener : 
	public CommandListenerBase<SubspaceServerCommandListener, SubspaceServerCommand>
{
public:

	//SubspaceServerCommandListener();
	//~SubspaceServerCommandListener();

	//virtual void handleCommand(SubspaceServerCommand* command);


	///////////////////////////////////////////////////////////////////////////
	// These functions are all no-ops, need to be implemented by subclass

	//0x01
	virtual void doServerArenaLogin(const string& arenaName, Uint8 startShip, Uint resX, Uint resY);	

	//0x02
	virtual void doServerArenaLeave();

	//0x03
	virtual void doServerPosition(Uint16 xPosition, Uint16 xVelocity, Uint16 yPosition, Uint16 yVelocity,
        Uint8 direction, Uint16 energy, Uint16 bounty, Uint8 togglables, Uint16 weaponData,
		Uint32 items, Uint16 latency, Uint16 timer, Uint32 timestamp);
	
	//0x05
	virtual void doServerDeath(Uint16 killerID, Uint16 bounty);

/*
	//0x07
	virtual void doServerChat(Uint16 playerID, const string& message, ChatType type, Uint8 soundByte);
*/

	//0x0A
	virtual void doServerPassword(const string& user, const string& password,	bool newUser);

	//0x0C
	virtual void doServerMapRequest();
	
	//0x0F
	virtual void doServerTeamChange(Uint16 teamID);

	//0x10
	virtual void doServerAttachRequest(Uint16 playerID);

	//0x13
	virtual void doServerFlagRequest(Uint16 flagID);
	
	//0x15
	virtual void doServerFlagsDrop();
	
	//0x17
	virtual void doServerRegistrationForm();
	
	//0x18
	virtual void doServerShipChange(ShipType ship);
	
	//0x1A
	virtual void doServerChecksumData(Uint32 arenaSettingsChecksum, 
		Uint32 mapChecksum, Uint32 exeChecksum);

	//0x1E
	virtual void doServerKothTimerEnded();

	//0x21
	virtual void doServerBrickDrop(Uint16 xTile, Uint16 yTile);
};

#endif