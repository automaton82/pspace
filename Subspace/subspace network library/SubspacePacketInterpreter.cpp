//David Millman

#include "SubspacePacketInterpreter.h"

#include "AsciiUtil.h"

//0x0002
void SubspacePacketInterpreter::encryptionResponse(const SubspacePacket &p, Uint32* encKey)
{
	if(encKey)
		*encKey = p.getDword(2);
}

//0x0003
void SubspacePacketInterpreter::reliablePacket(const SubspacePacket& p, Uint32* ackID, SubspacePacket* body)
{
	if(ackID)
		*ackID = p.getDword(2);

	//SubspacePacket pack = p;

	/*printf("PACKET COMPARISON\n");
	for(Uint i=0; i < p.getSize(); i++)
		printf("%d:%d ", p[i], pack[i]);*/

	if(body)
	{
		(*body).operator =(p.subStr(6));//.getSubpacket(6);*/
		//printf("substr: %s\n", (*body).getString(0).c_str());
		//string str = p.getString(6);
		/*(*body).setString(str, 0);

		if(str.size() != body->size())
		{
			printf("UNMATCHED SIZE\n");
		}
		else
		{
			for(int i=0; i < str.size(); ++i)
			{
				if((*body)[i] != str[i])
				{
					printf("UNMATCHED INDEX\n");
				}
			}
		}*/
	}
	//body = p.subStr(6);
}

//0x0005
void SubspacePacketInterpreter::syncRequest(const SubspacePacket& p, Uint32* timeStamp, Uint32* packetsSent, Uint32* packetsReceived)
{
	if(timeStamp)
		*timeStamp = p.getDword(2);

	if(packetsSent)
		*packetsSent = p.getDword(6);

	if(packetsReceived)
		*packetsReceived = p.getDword(10);
}

//0x0006
void SubspacePacketInterpreter::syncResponse(const SubspacePacket& p, Uint32* timestampAtRequester, Uint32* timestampAtRequestee)
{
	if(timestampAtRequester)
		*timestampAtRequester = p.getDword(2);

	if(timestampAtRequestee)
		*timestampAtRequestee = p.getDword(6);
}

//0x0008
void SubspacePacketInterpreter::smallChunk(const SubspacePacket& p, SubspacePacket* data)
{
	if(data)
	{
		*data = p.subStr(2);
		//(*data).setString(p.getString(2), 0);
	}
}

//0x0009
void SubspacePacketInterpreter::smallChunkTail(const SubspacePacket& p, SubspacePacket* data)
{
	smallChunk(p, data);
}

//0x000A
void SubspacePacketInterpreter::fileTransfer(const SubspacePacket& p, Uint32* fileSize, string* data)
{
	if(fileSize)
	{
		*fileSize = p.getDword(2);
	}

	if(data)
		*data = p.getString(6);
}

//0x01
void SubspacePacketInterpreter::playerID(const SubspacePacket& p, Uint16* playerID)
{
	if(playerID)
        *playerID = p.getWord(1);
}

//0x02
void SubspacePacketInterpreter::inGame(const SubspacePacket& p)
{

}

//0x03
void SubspacePacketInterpreter::playerEntering(const SubspacePacket& p, PlayerData* info, SubspacePacket* remainingPlayers)
{
	//Courtesy of Catid
	/*	Field	Length	Description
	The following are repeated until the end of the message
		0		1		Type byte
		1		1		Ship type
		2		1		Accepts audio messages
		3		20		Player name (confirmed ASCIIZ)
		23		20		Squad name (confirmed ASCIIZ)
		43		4		Kill points		- kill and flag points were reversed before (wrong), 
		47		4		Flag points  
		51		2		Player ident
		53		2		Team
		55		2		Wins
		57		2		Losses
		59		2		Turretee ident
		61		2		Flags carried
		63		1		Boolean: Has KoTH
	*/

	if(info)
	{
		info->shipType = p.getByte(1);
		info->acceptsAudio = p.getByte(2);

		info->name = AsciiUtil::clipWhitespace(p.getString(3, 20));	
		//strcpy(info->name, p.getString(3, 20).c_str());

		info->squadName = AsciiUtil::clipWhitespace(p.getString(23, 20));
		//strcpy(info->squadName, p.getString(23, 20).c_str());

		info->killPoints = p.getDword(43);
		info->flagPoints = p.getDword(47);
		info->playerID = p.getWord(51);
		info->team = p.getWord(53);
		info->wins = p.getWord(55);
		info->losses = p.getWord(57);
		info->turreteeID = p.getWord(59);
		info->flagsCarried = p.getWord(61);
		info->hasKoTH = p.getByte(63);
	}

	if(remainingPlayers)
	{
		if(p.getSize() > 64)
		{
			(*remainingPlayers).resize(p.getSize() - 64);
			(*remainingPlayers) = p.subStr(64);
			//(*remainingPlayers).setString(p.getString(64), 0);
		}
		else *remainingPlayers = SubspacePacket();
	}
}

//0x04
void SubspacePacketInterpreter::playerLeaving(const SubspacePacket& p, Uint16* playerID)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		2		Player ident
	*/

	if(playerID)
		*playerID = p.getWord(1);
}

//0x05
void SubspacePacketInterpreter::positionWeapon(const SubspacePacket& p, Uint16* playerID, Uint16* bounty, 
		Uint16* xPosition, Uint16* xVelocity, Uint16* yPosition, Uint16* yVelocity, Uint8* direction, 
		Uint8* togglables, Uint16* weaponInfo, Uint32* itemInfo, Uint16* energy, 	
		Uint16* timestamp, Uint8* checksum, Uint8* ping, Uint16* s2cLag, Uint16* timer)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		1		Rotation
		2		2		Timestamp
		4		2		X pixels
		6		2		Y velocity
		8		2		Player ident
		10		2		X velocity
		12		1		Checksum
		13		1		Togglables
		14		1		Ping
		15		2		Y pixels
		17		2		Bounty
		19		2		Weapon info
	Spectating with ExtraPositionData or *energy
		21		2		Energy
	Spectating with ExtraPositionData
		23		2		S2CLag
		25		2		Timer
		27		4		Item information
	*/

	if(direction)
		*direction = p.getByte(1);

	if(timestamp)
		*timestamp = p.getWord(2);
	
	if(xPosition)
		*xPosition = p.getWord(4);

	if(yVelocity)
		*yVelocity = p.getWord(6);

	if(playerID)
		*playerID = p.getWord(8);

	if(xVelocity)
		*xVelocity = p.getWord(10);

	if(checksum)
		*checksum = p.getByte(12);

	if(togglables)
		*togglables = p.getByte(13);

	if(yPosition)
		*yPosition = p.getWord(15);

	if(bounty)
		*bounty = p.getWord(17);

	if(weaponInfo)
		*weaponInfo = p.getWord(19);

	if(energy)
		*energy = p.getWord(21);

	if(s2cLag)
		*s2cLag = p.getWord(23);

	if(timer)
		*timer = p.getWord(25);

	if(itemInfo)
		*itemInfo = p.getDword(27);

}

//0x06
void SubspacePacketInterpreter::playerDeath(const SubspacePacket& p, Uint16* killerID, Uint16* playerKilledID, Uint16* bounty)
{	
	/*	Field	Length	Description
		0		1		Type byte
		1		1		? random
		2		2		Killer ident
		4		2		Killee ident
		6		2		Bounty
		8		2		?
	*/

	if(killerID)
		*killerID = p.getWord(2);

	if(playerKilledID)
		*playerKilledID = p.getWord(4);

	if(bounty)
		*bounty = p.getWord(6);
}

//0x07
void SubspacePacketInterpreter::chat(const SubspacePacket& p, Uint8* chatType, Uint8* soundByte, Uint16* originatorID, string* message)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		1		Message type
		2		1		Soundcode
		3		2		Player ident
		5		...\0	Message
	*/	

	if(chatType)
		*chatType = p.getByte(1);

	if(soundByte)
		*soundByte = p.getByte(2);

	if(originatorID)
		*originatorID = p.getWord(3);

	if(message)
		*message = p.getString(5);
}

//0x08
void SubspacePacketInterpreter::playerPrize(const SubspacePacket& p, Uint16* playerID, Uint16* xTiles, Uint16* yTiles, Uint16* prizeType, Uint32* timestamp)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		4		Timestamp
		5		2		X tiles
		7		2		Y tiles
		9		2		Prize type
		11		2		Player ident
	*/

	if(timestamp)
		*timestamp = p.getDword(1);

	if(xTiles)
		*xTiles = p.getWord(5);

	if(yTiles)
		*yTiles = p.getWord(7);

	if(prizeType)
		*prizeType = p.getWord(9);

	if(playerID)
		*playerID = p.getWord(11);
}


//0x09
void SubspacePacketInterpreter::scoreUpdate(const SubspacePacket& p, Uint16* playerID, Uint32* killPoints, Uint32* flagPoints, Uint16* wins, Uint16* losses)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		2		Player ident
		3		4		Flag points
		7		4		Kill points
		11		2		Wins
		13		2		Losses
	*/

	if(playerID)
		*playerID = p.getWord(1);

	if(killPoints)
		*killPoints = p.getDword(3);

	if(flagPoints)
		*flagPoints = p.getDword(7);

	if(wins)
		*wins = p.getWord(11);

	if(losses)
		*losses = p.getWord(13);

}

//0x0A
void SubspacePacketInterpreter::passwordResponse(const SubspacePacket &p, 
	Byte* loginResponse, 
	string* message,
	Uint32* serverVersion,
	Byte* regForm,
	Uint32* newsChecksum
	)
{
	/*
	Field	Length	Description
	0		1		Type byte
	1		1		Accept response meaning
	2		4		Server version
	6		4		?
	10		4		EXE checksum
	14		4		? unused
	18		1		? boolean
	19		1		Boolean: Request registration form
	20		4		? memory checksum
	24		4		News checksum (0 = no news file)
	28		4		? time/date
	32		4		? time/date
	*/
	
	//Get Login Response
	if(loginResponse)
		*loginResponse = p.getByte(1);
	//Get Server Version
	if(serverVersion)
		*serverVersion = p.getDword(2);
	//If the Server Requested a Reg Form (boolean)
	if(regForm)
	{
		if(p.getByte(19))
			*regForm = true;
		else
			*regForm = false;
	}
	//	*regForm = static_cast<bool>(p.getByte(19));
	//News File Checksum
	if(newsChecksum)
        *newsChecksum = p.getDword(24);

	if(!message) return;

	switch(p.getByte(1))
	{
		case 0: //Login OK
			*message = "Login Successful.";
			break;
		case 1: //Unkown Player
			*message = "Unknown (Unregistered) Player, Please register this name first.";
			break;
		case 2: //Bad Password
			*message = "Incorrect Password.";
			break;
		case 3: //Arena Full
			*message = "The Arena is full, please try again later.";
			break;
		case 4: //Locked Out
			*message = "You are locked out of SubSpace.";
			break;
		case 5: //Permision Only Arena
			*message = "You do not have Permision to play in this Arena.";
			break;
		case 6: //Spectate Only
			*message = "You only have permision to spectate in this Arena.";
			break;
		case 7: //Too many poits
			*message = "This name has too many points to play in this Arena.";
			break;
		case 8: //Connection too slow
			*message = "Your Connection is too slow to play in this Arena.";
			break;
		case 9: //Permision Only Arena (Same as 5)
			*message = "You do not have Permision to play in this Arena.";
			break;
		case 10: //Server Full
			*message = "The server is no longer accepting new connections. Try again later.";
			break;
		case 11: //Invalid Name
			*message = "The name you are trying to use is Inavlid, please choose another.";
			break;
		case 12: //Offensive Name
			*message = "The name you are trying to use is on the Offensive Word list, please choose another.";
			break;
		case 13: //No Biller (no socres)
			*message = "NOTICE: Scores are not being recorded in this zone. (Posible cause: No SS Biller Connection.)";
			break;
		case 14: //Server Busy
			*message = "Server busy proccessing other login requests. Try again later.";
			break;
		case 15: //Restricted Zone (Not enough usage)
			*message = "You do not have enough Usage. This zone is restricted to people with a certain number of hours logged on SS.";
			break;
		case 16: //Demo Version <- If you get this, theres a serious problem...
			*message = "You have bean detected as using the Demo Version of SubSpace.";
			break;
		case 17: //Too many Demo Users (Same as above applys)
			*message = "You have bean detected as using the Demo Version of SubSpace, too many Demo players at this time.";
			break;
		case 18: //No Demo Players Allowed (again same as the above two)
			*message = "You have bean detected as using the Demo Version of SubSpace, no Demo players allowed.";
			break;
		case 255: //Mod Access Required (Added by MGB)
			*message = "Moderator Access is required to enter this zone.";
			break;
		default:
			*message = "Unkown Login response";
			break;
	}
}

//0x0D
void SubspacePacketInterpreter::playerChangeTeam(const SubspacePacket& p, Uint16* playerID, Uint16* team, Uint8* ship)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		2		Player ident
		3		2		Team
		5		1		Ship changed, only if high bit is not set (Snrrrub)
	*/

	if(playerID)
		*playerID = p.getWord(1);

	if(team)
		*team = p.getWord(3);

	if(ship)
		*ship = p.getByte(5);
}


//0x0E
void SubspacePacketInterpreter::turretLink(const SubspacePacket& p, Uint16* requesterID, Uint16* destID)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		2		Turreter ident
		3		2		Turretee ident (when -1, detaching)
	*/

	if(requesterID)
		*requesterID = p.getWord(1);

	if(destID)
		*destID = p.getWord(3);
}

//0x0F
void SubspacePacketInterpreter::arenaSettings(const SubspacePacket& p, ArenaSettings* arena)
{
	/*	Field	Length	Description
		0		1428	Settings buffer
	The type byte is included in checksums
	*/

	if(arena)
		p.getData(arena, sizeof(ArenaSettings), 0);
}

//0x12
void SubspacePacketInterpreter::flagPosition(const SubspacePacket& p, Uint16* flagID, Uint16* xTiles, Uint16* yTiles, Uint16* team)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		2		Flag ident
		3		2		X tiles
		5		2		Y tiles
		7		2		Team
	*/

	if(flagID)
		*flagID = p.getWord(1);

	if(xTiles)
		*xTiles = p.getWord(3);
	
	if(yTiles)
		*yTiles = p.getWord(5);

	if(team)
		*team = p.getWord(7);
}

//0x13
void SubspacePacketInterpreter::flagClaim(const SubspacePacket& p, Uint16* flagID, Uint16* playerID)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		2		Flag ident
		3		2		Player ident
	*/

	if(*flagID)
		*flagID = p.getWord(1);

	if(*playerID)
		*playerID = p.getWord(3);
}

//0x15
void SubspacePacketInterpreter::destroyTurretLink(const SubspacePacket& p, Uint16* playerID)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		2		Player ident
	*/

	if(*playerID)
		*playerID = p.getWord(1);
}

//0x16
void SubspacePacketInterpreter::flagDrop(const SubspacePacket& p, Uint16* playerID)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		2		Player ident
	*/
	
	if(playerID)
		*playerID = p.getWord(1);
}


//0x18
void SubspacePacketInterpreter::serverSyncRequest(const SubspacePacket& p, 
	Uint32* prizeSeed, Uint32* doorSeed, Uint32* timestamp, Uint32* checksumKey)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		4		Green seed
		5		4		Door seed
		9		4		Timestamp (Kavar! mistook it for a seed)
		13		4		Checksum generator key
	*/
	
	if(prizeSeed)
		*prizeSeed = p.getDword(1);

	if(doorSeed)
		*doorSeed = p.getDword(5);

	if(timestamp)
		*timestamp = p.getDword(9);

	if(checksumKey)
		*checksumKey = p.getDword(13);
}

//0x1C
void SubspacePacketInterpreter::playerChangeSpectate(const SubspacePacket& p)
{
	/*	Field	Length	Description
		0		1		Type byte
	Either			Player joining spectator mode
		1		2		Player ident
	Or				Someone is requesting extra position information
		1		1		Boolean: send ExtraPositionInfo
	*/

	//TODO: finish this
}

//0x1D
void SubspacePacketInterpreter::playerChangeShip(const SubspacePacket& p, Uint8* ship, Uint16* playerID, Uint16* team)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		1		Ship type
		2		2		Player ident
		4		2		Team
	*/
	if(ship)
		*ship = p.getByte(1);

	if(playerID)
		*playerID = p.getWord(2);

	if(team)
		*team = p.getWord(4);
}

//0x1F
void SubspacePacketInterpreter::playerBanner(const SubspacePacket& p, Uint16* playerID, string* bannerData)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		2		Player ident
		3		96		Player banner
	*/

	if(playerID)
		*playerID = p.getWord(1);

	if(bannerData)
		*bannerData = p.getString(3, 96);
}

//0x21
void SubspacePacketInterpreter::brickDrop(const SubspacePacket& p, vector<BrickData>& bricks)
{
	/*	Field	Length	Description
		0		1		Type byte
	The following are repeated until the end of the message
		1		2		X1 tiles
		3		2		Y1 tiles
		5		2		X2 tiles
		7		2		Y2 tiles
		9		2		Team
		11		2		Brick ident (sent more than once)
		13		4		Timestamp
	*/

	BrickData brick;
	
	Uint size = p.size();
	Uint offset = 1;
	while(offset < size)
	{
		brick.x1 = p.getWord(offset);
		brick.y1 = p.getWord(offset + 2);
		brick.x2 = p.getWord(offset + 4);
		brick.y2 = p.getWord(offset + 6);
		brick.team = p.getWord(offset + 8);
		brick.brickId = p.getWord(offset + 10);
		brick.timestamp = p.getDword(offset + 12);

		offset += 16;

		bricks.push_back(brick);
	}
}

//0x27
void SubspacePacketInterpreter::keepAlive(const SubspacePacket& p)
{
}

//0x28
void SubspacePacketInterpreter::position(const SubspacePacket& p, Uint8* playerID, Uint8* bounty, 
	Uint16* xPosition, Uint16* xVelocity, Uint16* yPosition, Uint16* yVelocity, Uint8* direction, 
	Uint8* togglables, Uint32* itemInfo, Uint16* energy, 
	Uint16* timestamp, Uint8* ping, Uint16* s2cLag, Uint16* timer)
{
	
	/*	Field	Length	Description
		0		1		Type byte
		1		1		Rotation
		2		2		Time stamp
		4		2		X pixels
		6		1		Ping
		7		1		Bounty
		8		1		Player ident
		9		1		Togglables
		10		2		Y velocity
		12		2		Y pixels
		14		2		X velocity
	Spectating with ExtraPositionData or *energy
		16		2		Energy
	Spectating with ExtraPositionData
		18		2		S2CLag
		20		2		Timer
		22		4		Item information
	*/

	if(direction)
		*direction = p.getByte(1);

	if(timestamp)
		*timestamp = p.getWord(2);
	
	if(xPosition)
		*xPosition = p.getWord(4);

	if(ping)
		*ping = p.getByte(6);

	if(bounty)
		*bounty = p.getByte(7);

	if(playerID)
		*playerID = p.getByte(8);

	if(togglables)
		*togglables = p.getByte(9);

	if(yVelocity)
		*yVelocity = p.getWord(10);

	if(yPosition)
		*yPosition = p.getWord(12);

	if(xVelocity)
		*xVelocity = p.getWord(14);

	if(energy)
		*energy = p.getWord(16);

	if(s2cLag)
		*s2cLag = p.getWord(18);

	if(timer)
		*timer = p.getWord(20);

	if(itemInfo)
		*itemInfo = p.getWord(22);
}

//0x29
void SubspacePacketInterpreter::mapInfo(const SubspacePacket& p, string* name, 
	Uint32* mapChecksum, Uint32* downloadSize)
{
	// SubSpace's version:
	/*	Field	Length	Description
		0		1		Type byte
		1		16		Map name
		17		4		Map checksum
	Optional parameter when SS version != 134
		21		4		Map download size
	*/

	// Continuum's version:
	/*	Field	Length	Description
		0		1		Type byte
	The following are repeated until the end of the message
		1		16		File name
		17		4		File checksum
	*/

    if(name)
		*name = p.getString(1, 16);
	
	if(mapChecksum)
		*mapChecksum = p.getDword(17);

	if(downloadSize)
		*downloadSize = p.getDword(21);
}

//0x2A
void SubspacePacketInterpreter::mapFile(const SubspacePacket& p, string* mapName, string* mapData)
{
/*	Field	Length	Description
		0		1		Type byte
		1		16		Map name
		17		...		Compressed map
	*/

	if(mapName)
		*mapName = p.getString(1, 16);

	if(mapData)
		*mapData = p.getString(17);
}

//0x2B
void SubspacePacketInterpreter::kothTimerSet(const SubspacePacket& p, Uint32* time)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		4		New timer value
	*/

	if(time)
		*time = p.getDword(1);
}

//0x2C
void SubspacePacketInterpreter::kothReset(const SubspacePacket& p, Uint8* unknown, Uint32* time, Uint16* playerID)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		1		Adding KoTH timer
		2		4		Timer value
		6		2		Player ident
	*/

	if(unknown)
		*unknown = p.getByte(1);

	if(time)
		*time = p.getDword(2);

	if(playerID)
		*playerID = p.getWord(6);
}


//0x2E
void SubspacePacketInterpreter::ballPosition(const SubspacePacket& p, Uint8* ballID, Uint16* xPixel, 
	Uint16* yPixel, Sint16* xVelocity, Sint16* yVelocity, Uint16* ownerID, Uint16* timestamp)
{
/*	Field	Length	Description
		0		1		Type byte
		1		1		Powerball ident
		2		2		X pixels
		4		2		Y pixels
		6		2		X velocity
		8		2		Y velocity
		10		2		Owner ident
		12		4		Timestamp
	*/

	if(ballID)
		*ballID = p.getByte(1);

	if(xPixel)
		*xPixel = p.getWord(2);

	if(yPixel)
		*yPixel = p.getWord(4);

	if(xVelocity)
		*xVelocity = p.getWord(6);

	if(yVelocity)
		*yVelocity = p.getWord(8);

	if(ownerID)
		*ownerID = p.getWord(10);

	if(timestamp)
		*timestamp = p.getWord(12);
}

//0x2F
void SubspacePacketInterpreter::arenaList(const SubspacePacket& p, 
	vector<string>& names, vector<Uint>& population)
{
	/*	Field	Length	Description
		0		1		Type byte
	The following are repeated until the end of the message
		1		...\0	Arena name
		?		2		Arena population
	*/
	
	Uint size = p.getSize();
	Uint offset = 1;
	short pop;
	char c = 0;
	string name;

	while(offset < size)
	{
		name.clear();

		c = p.getByte(offset++);
		while(c != 0 && offset < size)
		{
			name += c;
			c = p.getByte(offset++);
		}
		
		pop = p.getWord(offset);
		offset += 2;

		names.push_back(name);
		population.push_back(pop);
	}
}
