#include "SubspaceGameProtocol.h"

#include "SubspaceChatCommands.h"
#include "SubspaceClientCommands.h"
#include "SubspaceGamePackets.h"

#include "PacketLog.h"

void SubspaceGameProtocol::onPacket(SubspacePacket& p)
{
	if(p.getByte(0) == 0)
	{
		//packetout << "SubspaceGameProtocol::handleReceivedPackets - Error: Core packet in main protocol" << endl;
		return;
	}

	//this->debugLog("PACKET: Packet Received, 0x%02x: ", p.getByte(0));
	
	switch(p.getByte(0))
	{
	case CPT_PlayerID:			onSetMyPlayerID(p);		break;
	case CPT_InGame:			onInGame(p);			break;
	case CPT_PlayerEntering:	onPlayerEntering(p);	break;
	case CPT_PlayerLeaving:		onPlayerLeaving(p);		break;
	case CPT_WeaponFired:		onPositionWeapon(p);	break;
	case CPT_PlayerDeath:		onPlayerDeath(p);		break;
	case CPT_Chat:				onChat(p);				break;
	case CPT_PlayerPrize:		onPlayerPrize(p);		break;
	case CPT_ScoreUpdate:		onScoreUpdate(p);		break;
	case CPT_PasswordResponse:	onPasswordResponse(p);	break;
	case CPT_PlayerChangeTeam:	onPlayerChangeTeam(p);	break;
	case CPT_TurretLinkRequest:	onTurretLink(p);		break;
	case CPT_ArenaSettings:		onArenaSettings(p);		break;
	case CPT_TurretLink:			onTurretLink(p);		break;
	case CPT_FlagPosition:		onFlagPosition(p);		break;
	case CPT_FlagClaim:			onFlagClaim(p);			break;
	case CPT_DestroyTurretLink:	onDestroyTurretLink(p);	break;
	case CPT_FlagDrop:			onFlagDrop(p);			break;
	case CPT_ServerSyncRequest:	onServerSyncRequest(p);	break;
	case CPT_SpectatePlayer:		break;//TODO: spectate player		
	case CPT_PlayerChangeShip:	onPlayerChangeShip(p);	break;
	case CPT_PlayerBanner:		onPlayerBanner(p);		break;
	case CPT_BrickPlaced:		onBrickDrop(p);			break;
	case CPT_KeepAlive:			onKeepAlive(p);			break;
	case CPT_Position:			onPosition(p);			break;
	case CPT_MapInfo:			onMapInfo(p);			break;
	case CPT_MapFile:			onMapFile(p);			break;
	case CPT_KothTimerSet:		onKothTimerSet(p);		break;
	case CPT_KothReset:			onKothReset(p);			break;
	case CPT_BallPosition:		onBallPosition(p);		break;
	case CPT_ArenaList:			onArenaList(p);			break;
	case CPT_PastLogin:			onPastLogin(p);			break;
	default:
		printf("UNKNOWN packet, type=0x%02x  size=%d\n", p.getByte(0), p.getSize());
	}
}

//0x01
void SubspaceGameProtocol::onSetMyPlayerID(SubspacePacket& p)
{
	SubspaceClientCommands::SetMyPlayerID* cmd = new SubspaceClientCommands::SetMyPlayerID;
	playerID(p, &cmd->id);
	queueClientCommand(cmd);

	PacketLog::log("queue: My playerID = %d\n", cmd->id);
}

//0x02
void SubspaceGameProtocol::onInGame(SubspacePacket& p)
{
	SubspaceClientCommands::InGame* cmd = new SubspaceClientCommands::InGame;
	inGame(p);  
	queueClientCommand(cmd);

	PacketLog::log("queue: In game.\n");
}

//0x03
void SubspaceGameProtocol::onPlayerEntering(SubspacePacket& p)
{
	/*	Field	Length	Description
	The following are repeated until the end of the message
		0		1		Type byte
		1		1		Ship type
		2		1		Accepts audio messages
		3		20		Player name (confirmed ASCIIZ)
		23		20		Squad name (confirmed ASCIIZ)
		43		4		Flag points
		47		4		Kill points
		51		2		Player ident
		53		2		Team
		55		2		Wins
		57		2		Losses
		59		2		Turretee ident
		61		2		Flags carried
		63		1		Boolean: Has KoTH
	*/
	//this->debugLog("Players entering: %d", p.getSize() / 64);
	
	SubspacePacket remaining = p;
	SubspacePacket others;

	do
	{
		SubspaceClientCommands::PlayerEntering* cmd = new SubspaceClientCommands::PlayerEntering;
		playerEntering(remaining, &cmd->p, &others);
		//if(strlen(cmd->p.name.c_str()) < cmd->p.name.size())
			cmd->p.name = cmd->p.name.c_str();

		//if(strlen(cmd->p.squad.c_str()) < cmd->p.squad.size())
			cmd->p.squadName = cmd->p.squadName.c_str();

		queueClientCommand(cmd);

		string tmp = cmd->p.name;
		PacketLog::log("queue: Player[%d] = %s\n", cmd->p.playerID, tmp.c_str());

		//this->log("Player entering: %s (%d)", data.info.name.c_str(), data.info.playerID);
		//debugout << "Player entering: " << data.info.name << " (" << data.info.playerID << ")" << endl;
	
		remaining = others;
	}
	while(remaining.getSize() > 0);
}

//0x04
void SubspaceGameProtocol::onPlayerLeaving(SubspacePacket& p)
{
	SubspaceClientCommands::PlayerLeaving* cmd = new SubspaceClientCommands::PlayerLeaving;
	playerLeaving(p, &cmd->id);
	queueClientCommand(cmd);

	PacketLog::log("queue: Player[%d] = NULL\n", cmd->id);
}

//0x05
void SubspaceGameProtocol::onPositionWeapon(SubspacePacket& packet)
{
	SubspaceClientCommands::PositionWeapon* cmd = new SubspaceClientCommands::PositionWeapon;
	positionWeapon(packet, &cmd->playerID, &cmd->bounty,
        (Uint16*)&cmd->posX, (Uint16*)&cmd->velX, (Uint16*)&cmd->posY, (Uint16*)&cmd->velY, &cmd->dir,
		&cmd->states.u, &cmd->weapon.u, &cmd->items.u, &cmd->energy, &cmd->timestamp, &cmd->checksum, &cmd->ping, 
		&cmd->s2cLag, &cmd->timer);
	queueClientCommand(cmd);

	PacketLog::log("queue: Player[%d] Update\n", cmd->playerID);
}

//0x06
void SubspaceGameProtocol::onPlayerDeath(SubspacePacket& p)
{
	SubspaceClientCommands::PlayerDeath* cmd = new SubspaceClientCommands::PlayerDeath;
	playerDeath(p, &cmd->killerID, &cmd->killedID, &cmd->bounty);
	queueClientCommand(cmd);

	PacketLog::log("queue: [%d] killed-> [%d](%d)\n", cmd->killerID, cmd->killedID, cmd->bounty);
}

//0x07
void SubspaceGameProtocol::onChat(SubspacePacket& p)
{	
	//SubspaceClientCommands::Chat* cmd = new SubspaceClientCommands::Chat; 
	SubspaceChatCommands::Chat* cmd = new SubspaceChatCommands::Chat; 
	chat(p, (Uint8*)&cmd->type, &cmd->soundByte, &cmd->playerID, &cmd->message);
	queueChatCommandInbound(cmd);

	string msg = cmd->message;
	PacketLog::log("queue: [%d]: \"%s\"\n", cmd->playerID, msg.c_str());
}

//0x08
void SubspaceGameProtocol::onPlayerPrize(SubspacePacket& p)
{
	SubspaceClientCommands::PlayerPrize* cmd = new SubspaceClientCommands::PlayerPrize;
	playerPrize(p, &cmd->playerID, &cmd->xTile, &cmd->yTile, &cmd->prizeType, &cmd->timestamp);
	queueClientCommand(cmd);

	PacketLog::log("queue: [%d] <- prize(%d, %d)\n", cmd->playerID, cmd->xTile, cmd->yTile);
}

//0x09
void SubspaceGameProtocol::onScoreUpdate(SubspacePacket& p)
{
	SubspaceClientCommands::ScoreUpdate* cmd = new SubspaceClientCommands::ScoreUpdate;
	scoreUpdate(p, &cmd->playerID, &cmd->killPoints, &cmd->flagPoints, &cmd->wins, &cmd->losses);
	queueClientCommand(cmd);

	PacketLog::log("queue: [%d] <- scoreUpdate(killPoints: %d, flagPoints: %d, wins: %d, losses: %d)\n", cmd->playerID, cmd->killPoints, cmd->flagPoints, cmd->wins, cmd->losses);
}

//0x0A
void SubspaceGameProtocol::onPasswordResponse(SubspacePacket& p)
{	
	SubspaceClientCommands::PasswordResponse* cmd = new SubspaceClientCommands::PasswordResponse;
	passwordResponse(p, &cmd->response, &cmd->message, &cmd->version, &cmd->regForm, &cmd->newsChecksum);
	queueClientCommand(cmd);

	PacketLog::log("queue: Login: \"%s\"\n", cmd->message.c_str());

	/*Uint8 response;
	string message;
	Byte regForm;
	Uint32 version, newsChecksum;
	
	passwordResponse(p, &response, &message, &version, &regForm, &newsChecksum);

	printf("Login response-\n");
	printf("\t%d: %s\n", response, message.c_str());
	printf("\tversion: %d\n", version);
	printf("\tregform: %d\n", regForm);
	printf("\tnews checksum: %d\n", newsChecksum);*/
}

//0x0D
void SubspaceGameProtocol::onPlayerChangeTeam(SubspacePacket& p)
{
	SubspaceClientCommands::PlayerChangeTeam* cmd = new SubspaceClientCommands::PlayerChangeTeam;
	playerChangeTeam(p, &cmd->playerID, &cmd->team, &cmd->ship);
	queueClientCommand(cmd);

	PacketLog::log("queue: Player[%d], team = %d\n", cmd->playerID, cmd->team);
}

//0x0E
void SubspaceGameProtocol::onTurretLink(SubspacePacket& p)
{
	SubspaceClientCommands::TurretLink* cmd = new SubspaceClientCommands::TurretLink;
	turretLink(p, &cmd->requesterID, &cmd->destID);
	queueClientCommand(cmd);
}

//0x0F
void SubspaceGameProtocol::onArenaSettings(SubspacePacket& p)
{
	SubspaceClientCommands::SetArenaSettings* cmd = new SubspaceClientCommands::SetArenaSettings;
	arenaSettings(p, &cmd->arenaSettings);
	queueClientCommand(cmd);

	PacketLog::log("queue: Arena settings.\n");
}

//0x12
void SubspaceGameProtocol::onFlagPosition(SubspacePacket& p)
{
	SubspaceClientCommands::FlagPosition* cmd = new SubspaceClientCommands::FlagPosition;
	flagPosition(p, &cmd->flagID, &cmd->xTile, &cmd->yTile, &cmd->teamID);
	queueClientCommand(cmd);

	PacketLog::log("queue: Flag[%d] @ (%d,%d)\n", cmd->flagID, cmd->xTile, cmd->yTile);
}

//0x13
void SubspaceGameProtocol::onFlagClaim(SubspacePacket& p)
{
	SubspaceClientCommands::FlagClaim* cmd = new SubspaceClientCommands::FlagClaim;
	flagClaim(p, &cmd->flagID, &cmd->playerID);
	queueClientCommand(cmd);
	PacketLog::log("queue: Player[%d] <- Flag[%d]\n", cmd->playerID, cmd->flagID);
}

//0x15
void SubspaceGameProtocol::onDestroyTurretLink(SubspacePacket& p)
{
	SubspaceClientCommands::DestroyTurretLink* cmd = new SubspaceClientCommands::DestroyTurretLink;
	destroyTurretLink(p, &cmd->playerID);
	queueClientCommand(cmd);
}


//0x16
void SubspaceGameProtocol::onFlagDrop(SubspacePacket& p)
{
	SubspaceClientCommands::FlagDrop* cmd = new SubspaceClientCommands::FlagDrop;
	flagDrop(p, &cmd->playerID);
	queueClientCommand(cmd);
}

//0x18
void SubspaceGameProtocol::onServerSyncRequest(SubspacePacket& p)
{
	SubspaceClientCommands::ServerSyncRequest* cmd = new SubspaceClientCommands::ServerSyncRequest;
	serverSyncRequest(p, &cmd->prizeSeed, &cmd->doorSeed, &cmd->timestamp, &cmd->checksumKey);
	queueClientCommand(cmd);

	PacketLog::log("queue: Server sync.\n");
}

//0x1D
void SubspaceGameProtocol::onPlayerChangeShip(SubspacePacket& p)
{
	SubspaceClientCommands::PlayerChangeShip* cmd = new SubspaceClientCommands::PlayerChangeShip;
	playerChangeShip(p, &cmd->ship, &cmd->playerID, &cmd->team);
	queueClientCommand(cmd);
}

//0x1F
void SubspaceGameProtocol::onPlayerBanner(SubspacePacket& p)
{
	SubspaceClientCommands::PlayerBanner* cmd = new SubspaceClientCommands::PlayerBanner;
	playerBanner(p, &cmd->playerID, &cmd->bannerData);
	queueClientCommand(cmd);
}

//0x21
void SubspaceGameProtocol::onBrickDrop(SubspacePacket& p)
{
	typedef SubspacePacketInterpreter::BrickData BrickData;

	list<BrickData> bricks;
	SubspaceClientCommands::BrickPlace* cmd = new SubspaceClientCommands::BrickPlace;
	brickDrop(p, cmd->bricks);

	queueClientCommand(cmd);
}

//0x27
void SubspaceGameProtocol::onKeepAlive(SubspacePacket& p)
{
	SubspaceClientCommands::KeepAlive* cmd = new SubspaceClientCommands::KeepAlive;
	queueClientCommand(cmd);

	PacketLog::log("queue: Keep alive.\n");
}

//0x28
void SubspaceGameProtocol::onPosition(SubspacePacket& packet)
{
	SubspaceClientCommands::Position* cmd = new SubspaceClientCommands::Position;
	position(packet, &cmd->playerID, &cmd->bounty,
        (Uint16*)&cmd->posX, (Uint16*)&cmd->velX, (Uint16*)&cmd->posY, (Uint16*)&cmd->velY, &cmd->dir, &cmd->stateInfo.u,
		&cmd->itemInfo.u, &cmd->energy, &cmd->timestamp, &cmd->ping, &cmd->s2cLag, &cmd->timer);
	queueClientCommand(cmd);
}

//0x29
void SubspaceGameProtocol::onMapInfo(SubspacePacket& p)
{
	SubspaceClientCommands::MapInfo* cmd = new SubspaceClientCommands::MapInfo;
	mapInfo(p, &cmd->mapFilename, &cmd->remoteFileChecksum, &cmd->downloadSize);
	
	string tmp = cmd->mapFilename;
	PacketLog::log("queue: Map[\"%s\"], size=%d\n", tmp.c_str(), cmd->downloadSize);

	queueClientCommand(cmd);
}


//0x2A
void SubspaceGameProtocol::onMapFile(SubspacePacket& p)	//always a compressed map file
{
	string mapName;
	string data;
	
	SubspaceClientCommands::MapFile* cmd = new SubspaceClientCommands::MapFile;
	mapFile(p, &cmd->mapName, &cmd->data);	//make this just pass filename?
	// TODO:save file?
	
	queueClientCommand(cmd);
}

//0x2B
void SubspaceGameProtocol::onKothTimerSet(SubspacePacket& p)
{
	SubspaceClientCommands::KothTimerSet* cmd = new SubspaceClientCommands::KothTimerSet;
	kothTimerSet(p, &cmd->time);
	queueClientCommand(cmd);
}

//0x2C
void SubspaceGameProtocol::onKothReset(SubspacePacket& p)
{
	SubspaceClientCommands::KothReset* cmd = new SubspaceClientCommands::KothReset;
	kothReset(p, 0, &cmd->time, &cmd->playerID);
	queueClientCommand(cmd);
}

//0x2E
void SubspaceGameProtocol::onBallPosition(SubspacePacket& p)
{
	SubspaceClientCommands::BallPosition* cmd = new SubspaceClientCommands::BallPosition;
	ballPosition(p, &cmd->ballID, &cmd->xPixel, &cmd->yPixel, &cmd->xVelocity, &cmd->yVelocity, &cmd->ownerID, &cmd->timestamp);
	queueClientCommand(cmd);
}

//0x2F
void SubspaceGameProtocol::onArenaList(SubspacePacket& p)
{
	SubspaceClientCommands::ArenaList* cmd = new SubspaceClientCommands::ArenaList;
	arenaList(p, cmd->names, cmd->population);
	queueClientCommand(cmd);
}

//0x31
void SubspaceGameProtocol::onPastLogin(SubspacePacket& p)
{
	SubspaceClientCommands::PastLogin* cmd = new SubspaceClientCommands::PastLogin;
	queueClientCommand(cmd);

	PacketLog::log("queue: Past login.\n");
}
