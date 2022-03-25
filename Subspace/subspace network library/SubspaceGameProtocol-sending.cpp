#include "SubspaceGameProtocol.h"

#define _USE_MATH_DEFINES
#include "math.h"

#include "AsciiUtil.h"
#include "SubspacePacketFactory.h"
using namespace SubspacePacketFactory;

#include "PacketLog.h"

//0x01
void SubspaceGameProtocol::doServerArenaLogin(const string& arenaName, Uint8 startShip, Uint resX, Uint resY)
{
	//Uint16 arenaType;
	string name = "";
	Uint num = 0;
	
	if(arenaName.size() == 0)
	{
		num = 0xFFFF;
	}
    else
	{
		if(AsciiUtil::isNumeric(arenaName))	//numbered arena
		{
			num = atoi(arenaName.c_str());
		}
		else	//named private arena
		{
			num = 0xFFFD;
			//num = 0xFFFF;
			name = arenaName;			
		}		
	}

	SubspacePacket p = arenaLogin(startShip, 1, resX, resY, num, name);		//TODO: make resolution not hardcoded

	//this->log("Logging in to arena \"%s\" (%d)", arenaName.c_str(), arenaNumber);
	//debugout << "Logging in to arena \"" << name << "\" (" << num << ")" << endl;

	printf("PACKET: Packet Sent, 0x01: Arena Login\n");
	//packetout << "PACKET: Packet sent, 0x01: Arena Login" << endl;
	coreProtocol_.sendPacketReliable(p);
}


//0x02
void SubspaceGameProtocol::doServerArenaLeave()
{
	SubspacePacket p = arenaLeave();

	//this->log("Leaving arena.");
	printf("PACKET: Packet Sent, 0x02: Arena Leave");
	coreProtocol_.sendPacketReliable(p);
}

//0x03
void SubspaceGameProtocol::doServerPosition(
	Uint16 xPosition, Uint16 xVelocity, Uint16 yPosition, Uint16 yVelocity, Uint8 direction, 
	Uint16 energy, Uint16 bounty, Uint8 togglables, Uint16 weaponInfo, Uint32 itemInfo,
	Uint16 latency, Uint16 timer, Uint32 timestamp)
{
	SubspacePacket packet = position(xPosition, xVelocity, yPosition, yVelocity, direction, 
		energy, bounty, togglables, weaponInfo, itemInfo, latency, timer, timestamp);

	if(weaponInfo == 0)
	{
		// printf("Game->: Position update.\n");	// this is done too often to display
	}
	else
	{
		// printf("Game->: Weapon fired.\n");
	}

	coreProtocol_.sendPacket(packet);	
}


//0x05
void SubspaceGameProtocol::doServerDeath(Uint16 killerID, Uint16 bounty)
{
	//this->log("killed by: %s (%d), %d", killerName.c_str(), player->getId(), bounty);
	SubspacePacket p = death(killerID, bounty);
	coreProtocol_.sendPacketReliable(p);
}

//0x06
void SubspaceGameProtocol::doServerChat(Uint16 playerID, const string& message, Uint8 type, Uint8 soundByte)
{
	PacketLog::log("Game->: [%d]::\"%s\"\n", playerID, message.c_str());

	SubspacePacket p = chat(type, 0, message, playerID);
	
	coreProtocol_.sendPacketReliable(p);
}

//0x09
void SubspaceGameProtocol::doServerPassword(const string& user, const string& pword, bool newUser)
{
	SubspacePacket p = password(newUser, user, pword);

	username_ = user;
	password_ = pword;

	printf("PACKET: Packet Sent, 0x09: Password\n");

	coreProtocol_.sendPacketReliable(p);
}

//0x10
void SubspaceGameProtocol::doServerAttachRequest(Uint16 playerID)
{
	SubspacePacket p = attachRequest(playerID);

	coreProtocol_.sendPacket(p);
}

//0x0C
void SubspaceGameProtocol::doServerMapRequest()
{
	SubspacePacket p = mapRequest();

	//this->log("Downloading file: %s\n", mapFilename.c_str());

	coreProtocol_.sendPacketReliable(p);
}

//0x0F
void SubspaceGameProtocol::doServerTeamChange(Uint16 team)
{
	SubspacePacket p = changeTeam(team);

	//this->log("Changed to team %d\n", team);
	
	coreProtocol_.sendPacketReliable(p);
}

//0x13
void SubspaceGameProtocol::doServerFlagRequest(Uint16 flagID)		//0x13
{
	SubspacePacket p = flagRequest(flagID);

	coreProtocol_.sendPacketReliable(p);
}

//0x15
void SubspaceGameProtocol::doServerFlagsDrop()		
{
	SubspacePacket p = flagsDrop();

	coreProtocol_.sendPacketReliable(p);
}

//0x17
void SubspaceGameProtocol::doServerRegistrationForm()
{
	SubspacePacket p = regFormResponse(
		"realName", "a@b.com", "city", "state", 0, 0, false, false, false, 586, 
		"windowsName", "windowsOrganization", "No desc.", "No desc.", "No desc.", "No desc.", 
		"No desc.", "No desc.", "No desc.", "No desc.", "No desc.", "No desc.", "No desc."
		"No desc.", "No desc.", "No desc.");

	coreProtocol_.sendPacket(p);
}

//0x18
void SubspaceGameProtocol::doServerShipChange(ShipType ship)	
{
	SubspacePacket p = setShipType(ship);

	coreProtocol_.sendPacketReliable(p);
}

//0x1A
void SubspaceGameProtocol::doServerChecksumData(Uint32 arenaSettingsChecksum, Uint32 mapChecksum, Uint32 exeChecksum)
{
	//TODO: finish real map download - do it correctly

	//string levelFilename = "levels/" + mapFilename;
	
	Uint pingCurr = coreProtocol_.getPingCurrent();
	Uint pingAve = coreProtocol_.getPingAverage();
	Uint pingLow = coreProtocol_.getPingLow();
	Uint pingHigh = coreProtocol_.getPingHigh();
	
	/*Uint pingCurr = 10;
	Uint pingAve = 10;
	Uint pingLow = 10;
	Uint pingHigh = 10;*/

	/*Uint32 keyTest, maxKey;
	maxKey = ~0;
	for(keyTest = 0; keyTest < maxKey; ++keyTest)
	{
		Uint32 a = checksum.exeChecksum(keyTest, exeFilename.c_str());
		Uint32 b = checksum.generateEXEChecksum(keyTest);
		assert(a == b);

		if(keyTest % 1000 == 0)
			printf("%d keys checked.\n", keyTest);
	}*/

	//TODO: make these work so there's not 100% weapon packetloss
	s2cSlowTotal_ = coreProtocol_.getPacketsReceived();
	s2cFastTotal_ = coreProtocol_.getPacketsReceived();
	s2cSlowCurrent_ = coreProtocol_.getPacketsReceived();
	s2cFastCurrent_ = coreProtocol_.getPacketsReceived();
	Uint16 reliableReceived = coreProtocol_.getReliableReceivedPackets();

	//packetout << "Slow total: " << s2cSlowTotal_ << ", Fast Total: " << s2cFastTotal_ << ", Slow Current: " << s2cSlowCurrent_ << ", Fast Current: " << s2cFastCurrent_ << endl;
	//packetout << "Reliable received: " << reliableReceived << endl;
	//packetout << "Weapon count: " << s2cWeaponCount_ << endl;
	/*printf("Slow total: %d, Fast Total: %d, Slow Current: %d, Fast Current: %d\n", s2cSlowTotal_, s2cFastTotal_, s2cSlowCurrent_, s2cFastCurrent_);
	printf("Reliable received: %d\n", reliableReceived);
	printf("Weapon count: %d\n", s2cWeaponCount_);*/

	SubspacePacket p = securityChecksum(
		s2cWeaponCount_,
		arenaSettingsChecksum, 
		exeChecksum,
		mapChecksum,
		s2cSlowTotal_, s2cFastTotal_, s2cSlowCurrent_, s2cFastCurrent_, 0, 
		pingCurr, pingAve,
		pingLow, pingHigh,
		false);

	//this->log("Sent security checksum (Synchronization response).");
	PacketLog::log("Game->: Security checksum.\n");

	//return true;

	//TODO: is this causing problems?
	coreProtocol_.sendPacket(p);
}

//0x1E
void SubspaceGameProtocol::doServerKothTimerEnded()
{
	SubspacePacket p = kothTimerEnded();

	coreProtocol_.sendPacketReliable(p);
}

//0x21 
void SubspaceGameProtocol::doServerBrickDrop(Uint16 xTile, Uint16 yTile)
{
	SubspacePacket p = brickDrop(xTile, yTile);

	coreProtocol_.sendPacket(p);
}
