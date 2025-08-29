#include "SubspaceCoreProtocol.h"

#ifndef _WIN32
#include <SDL2/SDL.h>
#endif

#include "SubspacePacketFactory.h"
using namespace SubspacePacketFactory;

#include "SubspacePacketInterpreter.h"
using namespace SubspacePacketInterpreter;

#include "PacketLog.h"


bool SubspaceCoreProtocol::sendPacket(SubspacePacket& p)
{
	// lock queue
	//	unnecessary
	// unlock queue

	return netHandler_.sendPacket(p);
}

bool SubspaceCoreProtocol::sendPacketReliable(SubspacePacket& p)
{
	//append reliable header
	//store packet
	int id = reliablePackets_.addFromClient(p);

	PacketLog::log("Core->: Reliable[%d]\n", id);

	return netHandler_.sendPacket(p);
}


//0x0001
bool SubspaceCoreProtocol::sendEncryptionRequest()
{
	Uint32 key = -(rand() % 0x7FFFFFFF);
	//Uint32 key = 0x2348985;

	sentKey_ = key;
	SubspacePacket p = encryptionRequest(key);

	//this->log("Sent encryption request.");
    PacketLog::log("Core->: Encryption Request, key=%08x\n", key);
	//return netHandler_.sendPacket(p);
	return sendPacket(p);
}

//0x0004
bool SubspaceCoreProtocol::sendReliableAck(Uint32 ackID)
{
	SubspacePacket ack = reliableAck(ackID);

	PacketLog::log("Core->: Reliable Ack: %d\n", ackID);
	return sendPacket(ack);
}

//0x0005
bool SubspaceCoreProtocol::sendSyncRequest()
{
#ifdef _WIN32
	SubspacePacket p = syncRequest(netHandler_.getPacketsSent(), netHandler_.getPacketsReceived(), SDL_GetTicks()/10);
#else
	SubspacePacket p = syncRequest(netHandler_.getPacketsSent(), netHandler_.getPacketsReceived(), SDL_GetTicks()/10);
#endif

	printf("Sending synchronization request...\n");

	return sendPacket(p);
}

//0x0006
bool SubspaceCoreProtocol::sendSyncResponse()
{
#ifdef _WIN32
	SubspacePacket p = syncResponse(serverTimeStamp_, SDL_GetTicks() / 10);
#else
	SubspacePacket p = syncResponse(serverTimeStamp_, SDL_GetTicks() / 10);
#endif

	//this->log("Sent synchronization response...");
	PacketLog::log("Core->: Sync Response\n");

	return sendPacket(p);
}

//0x0007
bool SubspaceCoreProtocol::sendDisconnect()
{
	SubspacePacket p = disconnect();

	//this->log("Disconnecting...");
	PacketLog::log("Core->: Disconnect\n");

	return sendPacket(p);
}

