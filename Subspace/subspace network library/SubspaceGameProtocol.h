//David Millman

#ifndef _SUBSPACEGAMEPROTOCOL_H_
#define _SUBSPACEGAMEPROTOCOL_H_

#include <windows.h>

#include <iostream>
#include <string>
//#include <fstream>

#include "CommandGenerator.h"
#include "DataTypes.h"
#include "Socket.h"
#include "Thread.h"

#include "Checksum.h"
#include "SubspaceCoreProtocol.h"
#include "SubspaceClientCommands.h"
#include "SubspaceServerCommandListener.h"
#include "SubspaceChatCommandReceiver.h"

#include "ChatSettings.h"
#include "WeaponSettings.h"


class SubspaceGameProtocol : 
	public CommandGenerator<SubspaceChatCommandReceiver>,
	public CommandGenerator<SubspaceClientCommandListener>,
	public SubspaceChatCommandReceiver,
	public SubspaceServerCommandListener
{
public:

	SubspaceGameProtocol();
	~SubspaceGameProtocol();

	// Connection
	bool connect(const string& user, const string& password, const string& host, Uint16 port, Uint timeout);
	bool connect2(const string& host, Uint16 port, Uint timeout);
	bool disconnect();
	bool isConnected() const;
	bool isRunning() const;

	// Accessors
	//Uint32 getPrizeSeed() const;
	//Uint32 getDoorSeed() const;

	// Mutators
	//void setPassword(const string& password);
	void setStartShip(ShipType ship);
	void setStartArena(const string& arenaName);
	//void setUsername(const string& username);

	// Receiving
	void executeInboundCommands();			// this is to decouple receiving thread from main thread											
											// receiveThread -> packetQueue
											//                  packetQueue -> mainThread
											// instead of
											// receiveThread -> packetQueue -> ChatReceiver/ClientReceiver
	//bool hasMoreCommands() const;
	//SubspaceClientCommand* nextCommand();	// caller is responsible for deleting memory
	//void handleReceivedPackets();				// actually does stuff with packets - called by zone

	// Sending
	void sendCommand(const SubspaceServerCommand* cmd);	// send to network

	virtual void doServerArenaLogin(const string& arenaName, Uint8 startShip, Uint resX, Uint resY);	//0x01
	virtual void doServerArenaLeave();								//0x02														
	virtual void doServerPosition(Uint16 xPosition, Uint16 xVelocity,		//0x03
		Uint16 yPosition, Uint16 yVelocity, Uint8 direction, Uint16 energy, 
		Uint16 bounty, Uint8 togglables, Uint16 weaponData, Uint32 items, 
		Uint16 latency, Uint16 timer, Uint32 timestamp);
	virtual void doServerDeath(Uint16 killerID, Uint16 bounty);		//0x05
	virtual void doServerChat(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte); //0x06	
	virtual void doServerPassword(const string& user, const string& password, bool newUser);	//0x0A
	virtual void doServerMapRequest();								//0x0C
	virtual void doServerTeamChange(Uint16 teamID);					//0x0F
	virtual void doServerAttachRequest(Uint16 playerID);			//0x10
	virtual void doServerFlagRequest(Uint16 flagID);				//0x13
	virtual void doServerFlagsDrop();								//0x15
	virtual void doServerRegistrationForm();						//0x17
	virtual void doServerShipChange(ShipType ship);					//0x18
	virtual void doServerChecksumData(Uint32 arenaSettingsChecksum, //0x1A
		Uint32 mapChecksum, Uint32 exeChecksum);
	virtual void doServerKothTimerEnded();							//0x1E
	virtual void doServerBrickDrop(Uint16 xTile, Uint16 yTile);		//0x21

private:

	void queueClientCommand(SubspaceClientCommand* cmd);		// from network, to client
	void queueServerCommand(SubspaceServerCommand* cmd);		// from client, to network
	void queueChatCommandInbound(SubspaceChatCommand* cmd);		// from network, to client
	void queueChatCommandOutbound(SubspaceChatCommand* cmd);	// from client, to network

    // Send packets, Command to SubspacePacket conversions
	//TODO: which packets should always be sent reliably?
	// 
	/*bool sendArenaLogin(const string& arenaName = "");	//0x01
	bool sendArenaLeave();								//0x02
	bool sendPosition(									//0x03
		Uint16 xPosition, Uint16 xVelocity, Uint16 yPosition, Uint16 yVelocity, Uint8 direction, 
		Uint16 energy, Uint16 bounty, Uint8 togglables, Uint16 weaponInfo, Uint32 itemInfo,
		Uint16 latency, Uint16 timer, Uint32 timestamp, Uint8 checksum);
	bool sendDeath(Uint16 killerID, Uint16 bounty);	//0x05
	bool sendChat(const string& name, const string& message, ChatType type);	//0x07
	bool sendChat(Uint16 playerID, const string& message, ChatType type);		//0x07
	bool sendAttachRequest(Uint16 playerID);			//0x10
	bool sendPassword(const string& user, const string& password, bool newUser = false);		//0x0A
	bool sendMapRequest();								//0x0C
	bool sendTeamChange(Uint16 team);					//0x0F
	bool sendFlagRequest(Uint16 flagID);				//0x13
	bool sendFlagsDrop();								//0x15
	bool sendRegForm();									//0x17
	bool sendShipChange(ShipType ship);					//0x18
	bool sendChecksum(Uint32 arenaSettingsChecksum, Uint32 mapChecksum, Uint32 exeChecksum);	//0x1A
	bool sendKothTimerEnded();							//0x1E
	bool sendBrickDrop(Uint16 xTile, Uint16 yTile);		//0x21*/
    
	// Handle packets (receiving), SubspacePacket to Command conversions
	void onSetMyPlayerID(SubspacePacket& p);		//0x01
	void onInGame(SubspacePacket& p);				//0x02
	void onPlayerEntering(SubspacePacket& p);		//0x03
	void onPlayerLeaving(SubspacePacket& p);		//0x04
	void onPositionWeapon(SubspacePacket& p);		//0x05
	void onPlayerDeath(SubspacePacket& p);			//0x06
	void onChat(SubspacePacket& p);					//0x07
	void onPlayerPrize(SubspacePacket& p);			//0x08
	void onScoreUpdate(SubspacePacket& p);			//0x09
	void onPasswordResponse(SubspacePacket& p);		//0x0A
	void onPlayerChangeTeam(SubspacePacket& p);	 	//0x0D
	void onTurretLink(SubspacePacket& p);			//0x0E
	void onArenaSettings(SubspacePacket& p);		//0x0F
	void onFlagPosition(SubspacePacket& p);			//0x12
	void onFlagClaim(SubspacePacket& p);			//0x13
	void onDestroyTurretLink(SubspacePacket& p);	//0x15
	void onFlagDrop(SubspacePacket& p);				//0x16
	void onServerSyncRequest(SubspacePacket& p);	//0x18
	void onPlayerChangeShip(SubspacePacket& p);		//0x1D
	void onPlayerBanner(SubspacePacket& p);			//0x1F
	void onBrickDrop(SubspacePacket& p);			//0x21
	void onKeepAlive(SubspacePacket& p);			//0x27
	void onPosition(SubspacePacket& p);				//0x28
	void onMapInfo(SubspacePacket& p);				//0x29
	void onMapFile(SubspacePacket& p);				//0x2A
	void onKothTimerSet(SubspacePacket& p);			//0x2B
	void onKothReset(SubspacePacket& p);			//0x2C
	void onBallPosition(SubspacePacket& p);			//0x2E
	void onArenaList(SubspacePacket& p);			//0x2F
	void onPastLogin(SubspacePacket& p);			//0x31
	//security packet
	//check purify

	void onPacket(SubspacePacket& p);	//puts a packet in receive queue

public:
	Uint16 myID;	//testing purposes only

private:

	friend void packetHandlerRoutine(void* ssProtocol);	//TODO: make this use a blocking operation

	typedef list<SubspaceClientCommand*> ClientCommandList;
	typedef list<SubspaceServerCommand*> ServerCommandList;
	typedef list<SubspaceChatCommand*> ChatCommandList;
	ClientCommandList clientCommands_;						// commands from the internet
	ServerCommandList serverCommands_;						// commands to the internet
	ChatCommandList chatCommandsInbound_, chatCommandsOutbound_;

	SubspaceCoreProtocol coreProtocol_;

	//packets	
	Thread packetHandlerThread_;

	Uint32 checksumKey_;
	Checksum checksum_;

	Uint s2cSlowTotal_, s2cFastTotal_;
	Uint s2cSlowCurrent_, s2cFastCurrent_;
	Uint s2cWeaponCount_;

	//stored connection information
	bool connected_;
	bool isRunning_;

	string username_;
	string password_;
	ShipType startShip_;
	string startArena_;
};

#endif