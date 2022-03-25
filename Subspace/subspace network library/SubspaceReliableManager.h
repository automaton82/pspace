//Adapted from Subspace Snrrrub's

#ifndef _SUBSPACERELIABLEMANAGER_H_
#define _SUBSPACERELIABLEMANAGER_H_

#include <map>
#include <windows.h>

#include "SubspacePacket.h"
#include "SubspacePacketFactory.h"
#include "SubspacePacketInterpreter.h"

class SubspaceReliableManager
{
public:
	//should be multimap?
	typedef std::map<Uint, SubspacePacket> PacketMap;

	SubspaceReliableManager();
	~SubspaceReliableManager();

	Uint getLastClientID() const;
	Uint getLastServerID() const;

	void addFromServer(Uint packetID, SubspacePacket& p);
	int getNextFromServer(SubspacePacket& p);
	bool availableFromServer() const;
	
	int addFromClient(SubspacePacket& p);
	bool removeFromClient(Uint packetID);
	SubspacePacket getNextFromClient(Uint currentTime, Uint resendInterval);
	
	bool availableFromClient(Uint currentTime, Uint resendInterval) const;

	void clear();

public:
   
    PacketMap clientPackets_;
	Uint clientPacketID_;
	Uint packetsSent_;

	PacketMap serverPackets_;	//TODO: should be linked list - drop packets less than last handled ID, don't store them
	Uint serverPacketID_;		//although, that might be a problem when packetId hits max Uint and it wraps around
};

#endif