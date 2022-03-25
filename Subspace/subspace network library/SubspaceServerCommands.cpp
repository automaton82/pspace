#include "SubspaceServerCommands.h"

#include "SubspaceServerCommandListener.h"

SubspaceServerCommand::~SubspaceServerCommand()
{
}

namespace SubspaceServerCommands
{

//0x01
void ArenaLogin::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerArenaLogin(arenaName, startShip, resX, resY);
}

//0x02
void ArenaLeave::execute(SubspaceServerCommandListener* listener)
{
}

//0x03
void Position::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerPosition(xPosition, xVelocity, yPosition, yVelocity, direction, energy, bounty,
        togglables, weaponData, items, latency, timer, timestamp);
}

//0x05
void Death::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerDeath(killerID, bounty);
}

//0x07
/*void Chat::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerChat(playerID, message, type, soundByte);
}
*/

//0x0A
void Password::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerPassword(user, password, newUser);
}

//0x0C
void MapRequest::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerMapRequest();
}

//0x0F
void TeamChange::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerTeamChange(teamID);
}

//0x10
void AttachRequest::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerAttachRequest(playerID);
}

//0x13
void FlagRequest::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerFlagRequest(flagID);
}

//0x15
void FlagsDrop::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerFlagsDrop();
}

//0x17
void RegistrationForm::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerRegistrationForm();
}

//0x18
void ShipChange::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerShipChange(ship);
}

//0x1A
void ChecksumData::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerChecksumData(arenaSettingsChecksum, mapChecksum, exeChecksum);
}

//0x1E
void KothTimerEnded::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerKothTimerEnded();
}

//0x21
void BrickDrop::execute(SubspaceServerCommandListener* listener)
{
	listener->doServerBrickDrop(xTile, yTile);
}

}