#ifndef _SUBSPACECOREPROTOCOL_H_
#define _SUBSPACECOREPROTOCOL_H_

#include <windows.h>

#include <list>
using std::list;

#include "Thread.h"

#include "SubspaceNetworkHandler.h"
#include "SubspaceReliableManager.h"

class SubspaceCoreProtocol
{
public:

	SubspaceCoreProtocol();
	~SubspaceCoreProtocol();

	bool init();

	// Layered functions
	bool connect(const string& host, Uint16 port);
	bool connect2(const string& host, Uint16 port);
	bool disconnect();
	bool isConnected() const;
	bool isRunning() const;

	// Accessors
	Uint getPacketsReceived() const;
	Uint getPingCurrent() const;
	Uint getPingLow() const;
	Uint getPingHigh() const;
	Uint getPingAverage() const;
	Uint getReliableSentPackets() const;
	Uint getReliableReceivedPackets() const;
	Uint32 getServerTimeStamp();

	// Mutators
	void setServerTimeStamp(Uint32 timestamp);	//main protocol needs to set this sometimes
	
	// Send packets
	bool sendEncryptionRequest();	//0x0001
	bool sendReliableAck(Uint32);	//0x0004
	bool sendSyncRequest();			//0x0005
	bool sendSyncResponse();		//0x0006
	bool sendDisconnect();			//0x0007

	// TODO: make this private
	// Send packet data
	bool sendPacket(SubspacePacket& p);
	bool sendPacketReliable(SubspacePacket& p);

	// Receive packets
	bool receivePacket(SubspacePacket& p);

private:

	// Handle packet types (receiving)
	bool onEncryptionResponse(SubspacePacket& p);	//0x0002
	bool onReliable(SubspacePacket& p);				//0x0003
	bool onReliableAck(SubspacePacket& p);			//0x0004
	bool onSyncRequest(SubspacePacket& p);			//0x0005
	bool onSyncResponse(SubspacePacket& p);			//0x0006
	bool onDisconnect(SubspacePacket& p);			//0x0007
	bool onSmallChunk(SubspacePacket& p);			//0x0008
	bool onSmallChunkTail(SubspacePacket& p);		//0x0009
	bool onFileTransfer(SubspacePacket& p);			//0x000A	// note: this is actually a large packet, e.g. a mapFile
	bool onCluster(SubspacePacket& p);				//0x000E

	void onPacket(SubspacePacket& p);				//main packet router

	// Threat routines
	friend void syncRoutine(void* coreProtocol);		//not used
	friend void reliablePacketHandlerRoutine(void* coreProtocol);
	// TODO: need packet gathering routine here

	typedef list<SubspacePacket> PacketList;

private:

	SubspaceNetworkHandler netHandler_;

	PacketList outboundPackets_;			//actually unnecessary //packets to the internet, from upper layer
	PacketList inboundPackets_;				//packets from the internet, passed to upper layer

	bool isConnected_;
	bool isRunning_;
	Uint32 sentKey_;		// for encryption

	// network stats
	Uint pingCurrent_;
	Uint pingLow_;
	Uint pingHigh_;
	Uint pingAverage_;

	Uint32 serverTimeStamp_;
	Uint32 clientTimeStamp_;

	SubspacePacket dataBuffer_;				// for packet chunks
	string fileBuffer_;						// for file transfers

	bool reliableResend_;
	Uint resendDelay_;
	SubspaceReliableManager reliablePackets_;
	Thread reliablePacketThread_;
};

#endif