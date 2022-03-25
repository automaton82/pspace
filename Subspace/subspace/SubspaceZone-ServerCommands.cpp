#include "SubspaceZone.h"

#include "Checksum.h"
#include "SubspacePlayer.h"

//0x01
bool SubspaceZone::sendArenaLogin(const string& arenaName)
{
	SubspaceServerCommands::ArenaLogin cmd;
	cmd.arenaName = arenaName;
	cmd.startShip = startShip_;

	this->CommandGenerator<SubspaceServerCommandListener>::invokeCommand(&cmd);
	return true;
}

//0x02
bool SubspaceZone::sendArenaLeave()
{
	return true;
}

//0x03
bool SubspaceZone::sendPosition()
{
	WeaponData w;
	w.u = 0;
	return sendWeapon(w);
}

//0x03
bool SubspaceZone::sendWeapon(const WeaponData& wep)
{
	SubspaceServerCommands::Position cmd;
	SubspacePlayer* p = this->getMyPlayer();
	if(!p)
	{
		printf("SubspaceZone::sendPosition(): No self player.\n");
		return true;
	}

	cmd.bounty = p->getBounty();
	cmd.direction = p->getSubspaceDirection();
	cmd.energy = p->getEnergy();
	cmd.items = p->getItemSettings().u;
	cmd.latency = 0;
	cmd.timer = p->getSuperTime();
	cmd.timestamp = GetTickCount();
	cmd.togglables = p->getStateSettings().u;
	cmd.weaponData = wep.u;
	cmd.xPosition = p->getSubspacePosition().x_;
	cmd.xVelocity = p->getSubspaceVelocity().x_;
	cmd.yPosition = p->getSubspacePosition().y_;
	cmd.yVelocity = p->getSubspaceVelocity().y_;

	this->CommandGenerator<SubspaceServerCommandListener>::invokeCommand(&cmd);

	return true;
}

//0x05
bool SubspaceZone::sendDeath(Uint16 killerID, Uint16 bounty)		
{
	SubspaceServerCommands::Death cmd;
	cmd.bounty = bounty;
	cmd.killerID = killerID;

	this->CommandGenerator<SubspaceServerCommandListener>::invokeCommand(&cmd);
	return true;
}

// bool sendChat(const string& name, const string& message, ChatType type);	//0x07
// bool sendChat(Uint16 playerID, const string& message, ChatType type);		//0x07

//0x10
bool SubspaceZone::sendAttachRequest(Uint16 playerID)
{
	// TODO: implement this
	return true;
}

//0x0A
bool SubspaceZone::sendPassword(const string& user, const string& password, bool newUser)
{
	SubspaceServerCommands::Password cmd;
	cmd.user = user;
	cmd.password = password;
	cmd.newUser = newUser;

	this->CommandGenerator<SubspaceServerCommandListener>::invokeCommand(&cmd);
	return true;
}

//0x0C
bool SubspaceZone::sendMapRequest()
{
	SubspaceServerCommands::MapRequest cmd;
	
	this->CommandGenerator<SubspaceServerCommandListener>::invokeCommand(&cmd);
	return true;
}

//0x0F
bool SubspaceZone::sendTeamChange(Uint16 team)
{
	SubspaceServerCommands::TeamChange cmd;
	cmd.teamID = team;

	this->CommandGenerator<SubspaceServerCommandListener>::invokeCommand(&cmd);

	return true;
}

//0x13
bool SubspaceZone::sendFlagRequest(Uint16 flagID)
{
	SubspaceServerCommands::FlagRequest cmd;
	cmd.flagID = flagID;

	this->CommandGenerator<SubspaceServerCommandListener>::invokeCommand(&cmd);

	return true;
}

//0x15
bool SubspaceZone::sendFlagsDrop()
{
	return true;
}

//0x17
bool SubspaceZone::sendRegForm()
{
	return true;
}

//0x18
bool SubspaceZone::sendShipChange(ShipType ship)
{
	SubspaceServerCommands::ShipChange cmd;
	cmd.ship = ship;

	this->CommandGenerator<SubspaceServerCommandListener>::invokeCommand(&cmd);

	return true;
}

//0x1A
bool SubspaceZone::sendChecksum(Uint32 checksumKey)
{
	Checksum checksum;
	SubspaceServerCommands::ChecksumData cmd;
	cmd.arenaSettingsChecksum = checksum.settingsChecksum(checksumKey, &arenaSettings_);
	cmd.exeChecksum = checksum.exeChecksum(checksumKey, "misc/subspace.bin");
		
	string levelFilename = zonePath_ + mapFilename_;
	char* mapData = new char[1024 * 1024];				// TODO: make it so the level isn't reloaded everytime

	Uint32 mapChecksum = 0;
	if(validMap_)
	{
		if(!checksum.LoadMap(levelFilename.c_str(), mapData))
			printf("Failed to load map file: \"%s\"\n", levelFilename.c_str());
		else
			mapChecksum = checksum.mapChecksum(checksumKey, mapData);
	}
	else
	{
		printf("No valid map for checksum.\n");
		cmd.mapChecksum = checksum.mapChecksum(checksumKey, mapData);
	}

	this->CommandGenerator<SubspaceServerCommandListener>::invokeCommand(&cmd);

	delete [] mapData;

	return true;
}

//0x1E
bool SubspaceZone::sendKothTimerEnded()
{
	return true;
}

//0x21
bool SubspaceZone::sendBrickDrop(Uint16 xTile, Uint16 yTile)
{
	SubspaceServerCommands::BrickDrop cmd;
	cmd.xTile = xTile;
	cmd.yTile = yTile;
	
	this->CommandGenerator<SubspaceServerCommandListener>::invokeCommand(&cmd);

	return true;
}
