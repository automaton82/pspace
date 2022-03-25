#include "SubspaceServerCommandListener.h"

#include "SubspaceServerCommands.h"

/*
SubspaceServerCommandListener::SubspaceServerCommandListener()
{}
SubspaceServerCommandListener::~SubspaceServerCommandListener()
{}
*/
/*void SubspaceServerCommandListener::handleCommand(SubspaceServerCommand* command)
{
	command->execute(this);
}*/
//0x01
void SubspaceServerCommandListener::doServerArenaLogin(const string& arenaName, Uint8 startShip, Uint resX, Uint resY){}
//0x02
void SubspaceServerCommandListener::doServerArenaLeave(){}
//0x03
void SubspaceServerCommandListener::doServerPosition(Uint16 xPosition, Uint16 xVelocity, Uint16 yPosition, Uint16 yVelocity,
    Uint8 direction, Uint16 energy, Uint16 bounty, Uint8 togglables, Uint16 weaponData,
	Uint32 items, Uint16 latency, Uint16 timer, Uint32 timestamp)
{}
//0x05
void SubspaceServerCommandListener::doServerDeath(Uint16 killerID, Uint16 bounty){}
/*
//0x07
void SubspaceServerCommandListener::doServerChat(Uint16 playerID, const string& message, ChatType type, Uint8 soundByte)
{}
*/
//0x0A
void SubspaceServerCommandListener::doServerPassword(const string& user, const string& password, bool newUser)
{}
//0x0C
void SubspaceServerCommandListener::doServerMapRequest()
{}
//0x0F
void SubspaceServerCommandListener::doServerTeamChange(Uint16 teamID)
{}
//0x10
void SubspaceServerCommandListener::doServerAttachRequest(Uint16 playerID)
{}
//0x13
void SubspaceServerCommandListener::doServerFlagRequest(Uint16 flagID)
{}
//0x15
void SubspaceServerCommandListener::doServerFlagsDrop()
{}
//0x17
void SubspaceServerCommandListener::doServerRegistrationForm()
{}
//0x18
void SubspaceServerCommandListener::doServerShipChange(ShipType ship)
{}
//0x1A
void SubspaceServerCommandListener::doServerChecksumData(Uint32 arenaSettingsChecksum, 
	Uint32 mapChecksum, Uint32 exeChecksum)
{}
//0x1E
void SubspaceServerCommandListener::doServerKothTimerEnded()
{}
//0x21
void SubspaceServerCommandListener::doServerBrickDrop(Uint16 xTile, Uint16 yTile)
{}