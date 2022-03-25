//David Millman

#include "SubspacePacketFactory.h"

//0x0001
SubspacePacket SubspacePacketFactory::encryptionRequest(Uint32 encryptionKey)
{
	/*
	Offset	Length	Descriptoin
	0		2		Type Byte 0x00,0x01
	2		4		Client Encryption Key
	6		2		Client Version/Type		// protocol version = 1 (SubSpace), or 16 (Continuum)
	*/
	
	SubspacePacket p(8);
	p.setByte(0x00, 0);
	p.setByte(0x01, 1);
	p.setDword(encryptionKey, 2);
	//p.setDword(0xF01E80C9, 2);
	p.setWord(0x0001, 6);			// subspace protocol
	//p.setWord(0x000F, 6);			// continuum protocol
	
	return p;
}


//0x0003
SubspacePacket SubspacePacketFactory::reliableHeader(Uint32 pID)
{
	SubspacePacket p(6);

	p.setByte(0x00, 0);
	p.setByte(0x03, 1);
	p.setDword(pID, 2);

	return p;
}

//0x0004
SubspacePacket SubspacePacketFactory::reliableAck(Uint32 pID)
{
	SubspacePacket p(6);

	p.setByte(0x00, 0);
	p.setByte(0x04, 1);
	p.setDword(pID, 2);

	return p;
}

//0x0005
SubspacePacket SubspacePacketFactory::syncRequest(Uint32 psSent, Uint32 psReceived, Uint32 timeStamp)
{
	/*
	Offset	Length	Descriptoin
	0		2		Type Byte 0x00,0x05
	2		4		Current Ticks (Machine Up Time in MS Divided by 10)
	6		4		Total number of ps Sent since Connnection
	10		4		Total number of ps Recieved since Connection
	*/
	
	SubspacePacket p(14);
	p.setByte(0x00, 0);
	p.setByte(0x05, 1);
	
	p.setDword(timeStamp, 2);
	p.setDword(psSent, 6);
	p.setDword(psReceived, 10);

	return p;
}

//0x0006
SubspacePacket SubspacePacketFactory::syncResponse(Uint32 serverTimeStamp, Uint32 clientTimeStamp)
{
	/*
	Offset	Length	Descriptoin
	0		2		Type Byte 0x00,0x06
	2		4		Servers Time Stamp
	6		4		Current Ticks (Machine Up Time in MS Divided by 10)
	*/
		
	SubspacePacket p(10);
	p.setByte(0x00, 0);
	p.setByte(0x06, 1);
	p.setDword(serverTimeStamp, 2);
	p.setDword(clientTimeStamp, 6);
	
	return p;
}

//0x0007
SubspacePacket SubspacePacketFactory::disconnect()
{
	/*
	Offset	Length	Descriptoin
	0		2		Type Byte 0x00,0x06
	2		4		Servers Time Stamp
	6		4		Current Ticks (Machine Up Time in MS Divided by 10)
	*/
		
	SubspacePacket p(2);
	p.setByte(0x00, 0);
	p.setByte(0x07, 1);
	
	return p;
}

//0x0008
SubspacePacket SubspacePacketFactory::smallChunk(const string& data)
{
	SubspacePacket p((Uint)data.size() + 2);
	p.setByte(0x00, 0);
	p.setByte(0x08, 1);
	p.setString(data, 2);
    
	return p;
}

//0x0009
SubspacePacket SubspacePacketFactory::smallChunkTail(const string& data)
{
	SubspacePacket p((Uint)data.size() + 2);
	p.setByte(0x00, 0);
	p.setByte(0x09, 1);
	p.setString(data, 2);
    
	return p;
}

//0x01
SubspacePacket SubspacePacketFactory::arenaLogin(
	Byte shipType, 
	Uint16 allowAudio,
	Uint16 resX,
	Uint16 resY,
	Uint16 arenaType,
	string arenaName
	)
{
	/*
	Offset	Length	Descriptoin
	0		1		Type Byte 0x01
	1		1		Ship type
	2		2		Allow audio?
	4		2		X resolution
	6		2		Y resolution
	8		2		Main arena number *1
	
	10		16		Arena name (Optional)

	*1 - Set to 0xFFFF for Random Pub,
	     or 0xFFFD for Specific Sub (Must provide name).
	*/
	
	SubspacePacket p(26);
	p.setByte(0x01, 0);
	p.setByte(shipType, 1);
	p.setWord(allowAudio, 2);
	p.setWord(resX, 4);
	p.setWord(resY, 6);
	p.setWord(arenaType, 8);
	//p.setString(arenaName.substr(0, 15) + "\0", 16);
	p.setString(arenaName, 10);

	return p;
}

//0x02
SubspacePacket SubspacePacketFactory::arenaLeave()
{
	/*	Field	Length	Description
		0		1		Type byte
	*/

	SubspacePacket p(1);
	p.setByte(0x02, 0);

	return p;
}

//0x03
SubspacePacket SubspacePacketFactory::position(
	Uint16 xPosition, Uint16 xVelocity, Uint16 yPosition, Uint16 yVelocity, Uint8 direction, 
	Uint16 energy, Uint16 bounty, Uint8 togglables, Uint16 weaponInfo, Uint32 itemInfo,
	Uint16 latency, Uint16 timer, Uint32 timestamp)
{
	/*	Field	Length	Description
	This is the extended version of the c2s position update
		0		1		Type byte
		1		1		Direction
		2		4		Timestamp
		6		2		X velocity
		8		2		Y pixels
		10		1		Checksum
		11		1		Togglables
		12		2		X pixels
		14		2		Y velocity
		16		2		Bounty
		18		2		Energy
		20		2		Weapon info
		22		2		Energy
		24		2		S2C latency
		26		2		Timer
		28		4		Item information
	*/

	SubspacePacket p(32);

	p.setByte(0x03, 0);
	p.setByte(direction, 1);
	p.setDword(timestamp, 2);
	p.setWord(xVelocity, 6);
	p.setWord(yPosition, 8); 
	p.setByte(0, 10); //Set this Later (Checksum)
	p.setByte(togglables, 11);
	p.setWord(xPosition, 12);
	p.setWord(yVelocity, 14);
	p.setWord(bounty, 16);
	p.setWord(energy, 18);
	p.setWord(weaponInfo, 20);
	
	//Create a Checksum Based off of the Current p Contents
	int Checksum = 0; //Creates and Inputs the Checksum
	for(int i = 0; i < 22; i++)
		Checksum ^= p[i];
	p.setByte(Checksum, 10);

	p.setWord(energy, 22);
	p.setWord(latency, 24);
	p.setWord(timer, 26);
	p.setDword(itemInfo, 28);
	
	
	return p;
}

//0x05
SubspacePacket SubspacePacketFactory::death(Uint16 killerID, Uint16 bounty)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		2		Killer ident
		3		2		Bounty
	*/

	SubspacePacket p(5);
	p.setByte(0x05, 0);
	p.setWord(killerID, 1);
	p.setWord(bounty, 3);

	return p;
}

//0x06
SubspacePacket SubspacePacketFactory::chat(Uint8 chatType, Uint8 soundByte, string text, Uint16 playerID)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		1		Chat type
		2		1		Soundcode
		3		2		Player ident
		5		...\0	Message
	*/

	if(text.length() > 200) text.resize(200);	// TODO: why is this limitation here?
	
	SubspacePacket p((Uint)text.size() + 6);
	
	p.setByte(0x06, 0);
	p.setByte(chatType, 1);
	p.setByte(soundByte, 2);
	p.setWord(playerID, 3);
	p.setString(text.c_str(), 5, 200);

	return p;
}

//0x09
SubspacePacket SubspacePacketFactory::password(bool newUser, string username, string password)
{
/*
	Offset	Length	Description
	0		1		Type byte
	1		1		Boolean: New user *1
	2		32		Name
	34		32		Password
	66		4		Machine ident *2
	70		1		ConnectType *3
	71		2		Timezone bias
	73		2		?
	75		2		Client version *4
	77		4		? memory checksum, Set to = 444
	81		4		? memory checksum, Set to = 555
	85		4		Permission ident
	89		12		?

	*1 - 1 = New, 0 = Not New

	*2 - Should be Drive Serial of C (can be random for bots)

	*3 - Type 0x00 is a safe bet.

	*4 - 0x24 = Ctm, 0x86 = SS
	*/
	
	//Create p and fill in a few initial values
	SubspacePacket p(101);
	p.setByte(0x09, 0);
	p.setByte(newUser, 1);

	//Create a Random number for use with certain IDs
	Uint32 RandomID = (rand() % 0x7FFFFFFE + 1);
		
	//Dump the User Name into the p
	p.setString(username, 2, 32);
	//Dump the User Pass into the p
	p.setString(password, 34, 32);
	//Dump various other info into the p
	p.setDword(RandomID, 66); //Machine ID
	p.setByte(0x00, 70); //Connection Type
	p.setWord(240, 71); //Time Zone Bias (240 = EST)
	p.setByte(0x00, 73); //Unkown.
	p.setByte(134, 75); //Client Type - subspace
	//p.setByte(38, 75); //Client Type - continuum
	p.setDword(444, 77); //Memory Checksums
	p.setDword(555, 81); //Memory Checksums
	//p.setDword(RandomID, 85); //Permission ID
	p.setDword(0x00, 85); //Permission ID

	return p;
}

//0x0C
SubspacePacket SubspacePacketFactory::mapRequest()
{
	SubspacePacket p(1);

	p.setByte(0x0C, 0);

	return p;
}

//0x0F
SubspacePacket SubspacePacketFactory::changeTeam(Uint16 newTeam)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		2		Team
	*/

	SubspacePacket p(3);

	p.setByte(0x0F, 0);
	p.setWord(newTeam, 1);

	return p;
}

//0x10
SubspacePacket SubspacePacketFactory::attachRequest(Uint16 playerID)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		2		Player ident
	*/

	SubspacePacket p(3);

	p.setByte(0x10, 0);
	p.setWord(playerID, 1);

	return p;
}

//0x13
SubspacePacket SubspacePacketFactory::flagRequest(Uint16 flagID)
{
	
	/*	Field	Length	Description
		0		1		Type byte
		1		2		Flag ident
	*/

	SubspacePacket p(3);

	p.setByte(0x13, 0);
	p.setWord(flagID, 1);

	return p;
}

//0x15
SubspacePacket SubspacePacketFactory::flagsDrop()
{
	
	/*	Field	Length	Description
		0		1		Type byte
	*/

	SubspacePacket p(1);

	p.setByte(0x15, 0);

	return p;
}

//0x17
SubspacePacket SubspacePacketFactory::regFormResponse(string realName, string email, string city, string state,
	Uint8 gender, Uint8 age, bool connectFromHome, bool connectFromWork, bool connectFromSchool,
	Uint32 processorType, string windowsRealName, string organization,
	string displayDesc, string monitorDesc, string modem1Desc, string modem2Desc, string mouseDesc, 
	string net00Desc, string net01Desc, string printerDesc, 
	string media0Desc, string media1Desc, string media2Desc, string media3Desc, string media4Desc)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		32		Real name
		33		64		Email
		97		32		City
		129		24		State
		153		1		Sex('M'/'F')
		154		1		Age*/

	SubspacePacket p(766);

	p.setByte(0x17, 0);
	p.setString(realName, 1, 32);
	p.setString(email, 33, 64);
	p.setString(city, 97, 32);
	p.setString(state, 129, 24);
	p.setByte(gender, 153);
	p.setByte(age, 154);

	/*
	Connecting from...
		155		1		Home
		156		1		Work
		157		1		School
	System information
		158		4		Processor type (586)
		162		2		?
		164		2		?
	Windows registration information (SSC RegName ban)
		166		40		Real name
		206		40		Organization*/

	p.setByte(connectFromHome, 155);
	p.setByte(connectFromWork, 156);
	p.setByte(connectFromSchool, 157);
	p.setDword(processorType, 158);
	p.setWord(0, 162);
	p.setWord(0, 164);
	p.setString(windowsRealName, 166, 40);
	p.setString(organization, 206, 40);
	
	/*Windows NT-based OS's do not send any hardware information (DreamSpec HardwareID ban)
		246		40		System\CurrentControlSet\Services\Class\Display\0000
		286		40		System\CurrentControlSet\Services\Class\Monitor\0000
		326		40		System\CurrentControlSet\Services\Class\Modem\0000
		366		40		System\CurrentControlSet\Services\Class\Modem\0001
		406		40		System\CurrentControlSet\Services\Class\Mouse\0000
		446		40		System\CurrentControlSet\Services\Class\Net\0000
		486		40		System\CurrentControlSet\Services\Class\Net\0001
		526		40		System\CurrentControlSet\Services\Class\Printer\0000
		566		40		System\CurrentControlSet\Services\Class\MEDIA\0000
		606		40		System\CurrentControlSet\Services\Class\MEDIA\0001
		646		40		System\CurrentControlSet\Services\Class\MEDIA\0002
		686		40		System\CurrentControlSet\Services\Class\MEDIA\0003
		726		40		System\CurrentControlSet\Services\Class\MEDIA\0004
	*/

	p.setString(displayDesc, 246, 40);
	p.setString(monitorDesc, 286, 40);
	p.setString(modem1Desc, 326, 40);
	p.setString(modem2Desc, 366, 40);
	p.setString(mouseDesc, 406, 40);
	p.setString(net00Desc, 446, 40);
	p.setString(net01Desc, 486, 40);
	p.setString(printerDesc, 526, 40);
	p.setString(media0Desc, 566, 40);
	p.setString(media1Desc, 606, 40);
	p.setString(media2Desc, 646, 40);
	p.setString(media3Desc, 686, 40);
	p.setString(media4Desc, 726, 40);

	return p;
}

//0x18
SubspacePacket SubspacePacketFactory::setShipType(Uint8 shipType)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		1		Ship type
	*/

	
	SubspacePacket p(2);

	p.setByte(0x18, 0);
	p.setByte(shipType, 1);
	
	return p;
}

//0x1A
SubspacePacket SubspacePacketFactory::securityChecksum(Uint32 weaponCount, Uint32 settingsChecksum, Uint32 subspaceEXEChecksum,
	Uint32 mapChecksum, Uint32 s2cSlowTotal, Uint32 s2cFastTotal, Uint16 s2cSlowCurrent,
	Uint16 s2cFastCurrent, Uint16 s2cRelOut, Uint16 ping, Uint16 pingAve, Uint16 pingLow, Uint16 pingHigh,
	bool slowFrameRate)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		4		Weapon count
		5		4		Parameter checksum
		9		4		EXE checksum
		13		4		Level checksum
		17		4		S2CSlowTotal
		21		4		S2CFastTotal
		25		2		S2CSlowCurrent
		27		2		S2CFastCurrent
		29		2		? S2CRelOut
		31		2		Ping
		33		2		Average ping
		35		2		Low ping
		37		2		High ping
		39		1		Boolean: Slow frame detected
	*/

	SubspacePacket p(40);

	p.setByte(0x1A, 0);
	p.setDword(weaponCount, 1);
	p.setDword(settingsChecksum, 5);
	p.setDword(subspaceEXEChecksum, 9);
	p.setDword(mapChecksum, 13);
	p.setDword(s2cSlowTotal, 17);
	p.setDword(s2cFastTotal, 21);
	p.setWord(s2cSlowCurrent, 25);
	p.setWord(s2cFastCurrent, 27);
	p.setWord(s2cRelOut, 29);
	p.setWord(ping, 31);
	p.setWord(pingAve, 33);
	p.setWord(pingLow, 35);
	p.setWord(pingHigh, 37);
	p.setByte(slowFrameRate, 39);

	return p;
}


//0x1C
SubspacePacket SubspacePacketFactory::brickDrop(Uint16 xTile, Uint16 yTile)
{
	/*	Field	Length	Description
		0		1		Type byte
		1		2		X tiles
		3		2		Y tiles
	*/

	SubspacePacket p(5);

	p.setByte(0x1C, 0);
	p.setWord(xTile, 1);
	p.setWord(yTile, 3);

	return p;
}

//0x1E
SubspacePacket SubspacePacketFactory::kothTimerEnded()
{	
	/*	Field	Length	Description
		0		1		Type byte
	*/

	SubspacePacket p(1);

	p.setByte(0x1E, 0);

	return p;
}