//David Millman

#include "SubspaceReliableManager.h"

SubspaceReliableManager::SubspaceReliableManager() :
	clientPacketID_(0),
	serverPacketID_(0),

	packetsSent_(0)
{
}

SubspaceReliableManager::~SubspaceReliableManager()
{
}

Uint SubspaceReliableManager::getLastClientID() const
{
	//TODO: this will cause a problem after the Uint wraps around
	return clientPacketID_;
}

Uint SubspaceReliableManager::getLastServerID() const
{
	//TODO: this will cause a problem after the Uint wraps around
	return serverPacketID_;
}

void SubspaceReliableManager::addFromServer(Uint packetID, SubspacePacket& p)
{
	//serverPackets_.insert(std::make_pair(packetID, p));
	serverPackets_[packetID] = p;
}

int SubspaceReliableManager::getNextFromServer(SubspacePacket& p)
{
	PacketMap::iterator i = serverPackets_.find(serverPacketID_);
	if(i != serverPackets_.end())
	{
		p = i->second;
		int id = i->first;
		serverPackets_.erase(i);

		++serverPacketID_;
		return id;
	}

	return -1;
}

bool SubspaceReliableManager::availableFromServer() const
{
	PacketMap::const_iterator i = serverPackets_.find(serverPacketID_);

	if(i != serverPackets_.end())
		return true;
	else
		return false;
}

int SubspaceReliableManager::addFromClient(SubspacePacket& p)
{
	Uint id;

	if(p.getSize() <= 6 || p[0] != 0x00 || p[1] != 0x03)
	{
		SubspacePacket relHeader = /*SubspacePacketFactory::*/packetFactory.reliableHeader(clientPacketID_++);
		p.prepend(relHeader);
	}
	
	clientPackets_[GetTickCount() / 10] = p;
	/*SubspacePacketInterpreter::*/packetInterpreter.reliablePacket(p, &id, NULL);
	return id;
}

bool SubspaceReliableManager::removeFromClient(Uint packetID)
{
	PacketMap::iterator i;

	for(i = clientPackets_.begin(); i != clientPackets_.end(); i++)
	{
		if( i->second.getDword(2) == packetID)
		{
			clientPackets_.erase(i);
			return true;
		}
	}
	return false;
}

bool SubspaceReliableManager::availableFromClient(Uint currentTime, Uint resendInterval) const
{
	if(clientPackets_.begin() == clientPackets_.end())
		return false;

	if(currentTime >= clientPackets_.begin()->first + resendInterval)
		return true;
	else return false;
}

SubspacePacket SubspaceReliableManager::getNextFromClient(Uint currentTime, Uint resendInterval)
{
	PacketMap::iterator i = clientPackets_.begin();
	if(availableFromClient(currentTime, resendInterval))
	{
		SubspacePacket p = i->second;
		clientPackets_.erase(i);
		return p;
	}
	else return SubspacePacket();
}

void SubspaceReliableManager::clear()
{
    clientPacketID_ = 0;
	serverPacketID_ = 0;

	serverPackets_.clear();
	clientPackets_.clear();
}