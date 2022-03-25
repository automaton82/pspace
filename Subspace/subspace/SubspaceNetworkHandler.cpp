//David Millman

#include "SubspaceNetworkHandler.h"

#include "WinUtil.h"

SubspaceNetworkHandler::SubspaceNetworkHandler()
	//reliablePacketThread(static_cast<Thread::startRoutine>(reliablePacketHandlerRoutine))
{
	//packetHandlerThread = INVALIDHANDLEVALUE;
	//positionThread = INVALIDHANDLEVALUE;
	encrypt = false;
	key = 0;

	packetsSent = 0;
	bytesSent = 0;
	packetsReceived = 0;

	connectionTest_ = false;

	timeout_ = 10000;
}

SubspaceNetworkHandler::~SubspaceNetworkHandler()
{
	//reliablePacketThread_.stop();
	disconnect();
}

bool SubspaceNetworkHandler::connect(const string& host, Uint16 port)
{
	if(!socket_.initUDP(host, port))
	{
		this->debugLog("UDP startup failed!\n");
		return false;
	}
	
	struct sockaddr_in server;
	struct hostent* hostEntry = gethostbyname(host.c_str());

	memcpy((char*)&server.sin_addr, (char*)hostEntry->h_addr, hostEntry->h_length);
	string ip = inet_ntoa(server.sin_addr);

	char buf[64];
	strcpy(buf, host.c_str());

	char buf2[64];
	strcpy(buf2, ip.c_str());
	//this->debugLog("Started connection for %s, %s:%d", host.c_str(), ip.c_str(), port);
	this->debugLog("Connecting to %s:%d...", buf2, port);

	return true;
}

bool SubspaceNetworkHandler::disconnect()
{
	return socket_.close();
}

void SubspaceNetworkHandler::setEncryptionKey(Uint32 k)
{
	this->key = k;
	this->makeStream(key);
}

void SubspaceNetworkHandler::setEncryptionStatus(bool doEncrypt)
{
	encrypt = doEncrypt;
}

Uint32 SubspaceNetworkHandler::getEncryptionKey()
{
	return key;
}

bool SubspaceNetworkHandler::getEncryptionStatus()
{
	return encrypt;
}

Uint SubspaceNetworkHandler::getPacketsSent() const
{
	return packetsSent;
}

Uint SubspaceNetworkHandler::getPacketsReceived() const
{
	return packetsReceived;
}

Uint SubspaceNetworkHandler::getBytesSent() const
{
	return bytesSent;
}

Uint SubspaceNetworkHandler::getBytesReceived() const
{
	return bytesReceived;
}

void SubspaceNetworkHandler::setConnectionTest(bool test)
{
	connectionTest_ = test;
}

bool SubspaceNetworkHandler::sendPacket(SubspacePacket& p)
{
	if(p.size() > MAX_PACKET_SIZE)
	{
		//return true;
		return sendPacketHelper(p);
	}

	++packetsSent;
	bytesSent += p.getSize();

	if(encrypt)
		encryptPacket(p);

	char buffer[MAX_PACKET_SIZE];
	int bufferSize = p.getSize();

	p.getData(buffer, bufferSize);
	
	if(rand() % 10 != 0 || !connectionTest_)
	{
		if(!socket_.sendData(buffer, bufferSize))
		{
			this->log("Packet send failed (SubspaceNetworkHandler - sendPacket)!");
			return false;
		}
		else return true;	
	}
	else return true;
}

bool SubspaceNetworkHandler::sendPacketHelper(SubspacePacket& p)
{
	if(p.size() <= MAX_PACKET_SIZE)
		return sendPacket(p);

	SubspacePacket chunk;
	Uint offset = 0;
	while(offset < p.size())
	{
		//if(p.size() - offset < MAX_PACKET_SIZE)
		if(p.size() < MAX_PACKET_SIZE + offset)
		{
			int size = p.size() - offset;
			if(size > 518) size = 518;
			chunk = /*SubspacePacketFactory::*/packetFactory.smallChunkTail(p.getString(offset, size));
		}
		else
		{
			int size = p.size() - offset;
			if(size > 518) size = 518;
			chunk = /*SubspacePacketFactory::*/packetFactory.smallChunk(p.getString(offset, size));
		}

		if(!sendPacket(chunk))
			return false;

		offset += 518;
	}

	return true;
}

/*bool SubspaceNetworkHandler::sendPacketReliable(SubspacePacket p)
{
	if(p.getSize() == 0) return false;

	if(p.size()+6 > MAX_PACKET_SIZE)
	{
		//return true;
		return sendPacketReliableHelper(p);
	}

	reliablePackets.addFromClient(p);

	Uint32 id;
	packetInterpreter.reliablePacket(p, &id, NULL);
	this->debugLog("Reliable packet sent: %d", id);
	
	return sendPacket(p);
}*/


/*bool SubspaceNetworkHandler::sendPacketReliableHelper(SubspacePacket& p)
{
	//TODO Test this function out
	if(p.size()+6 <= MAX_PACKET_SIZE)
		return sendPacketReliable(p);

	Uint offset = 0;
	while(offset < p.size())
	{
		SubspacePacket chunk;
		//if(p.size()-6 - offset < MAX_PACKET_SIZE)
		if(p.size() < MAX_PACKET_SIZE + 6 + offset)
		{
			int size = p.size() - offset - 6;
			if(size > 518 - 6) size = 518 - 6;
			chunk = /packetFactory.smallChunkTail(p.getString(offset, size));
		}
		else
		{
			int size = p.size() - offset - 6;
			if(size > 518 - 6) size = 518 - 6;
			chunk = packetFactory.smallChunk(p.getString(offset, size));
		}

		if(!sendPacketReliable(chunk))
			return false;

		offset += 518 - 6;
	}

	return true;
}*/

bool SubspaceNetworkHandler::receivePacket(SubspacePacket& p)
{
	char data[MAX_PACKET_SIZE];
	int bytes;

	if(!socket_.waitForData(timeout_))
	{
		this->log("No data coming from server... (%d ms since last packet)", timeout_);
		return false;
	}
	if(!socket_.receiveData(data, MAX_PACKET_SIZE, &bytes))
	{
		debugLog("PACKET: Packet receive failed (SubspaceNetworkHandler - receivePacket)!");
		return false;
	}		
	p.resize(bytes);
	p.setData(data, bytes, 0);

	packetsReceived++;
	bytesReceived += p.getSize();

	if(encrypt)
		decryptPacket(p);

	if(p.getSize() < 0)
		return false;

	return true;
}

/*bool SubspaceNetworkHandler::handleAck(Uint32 ackID)
{
	return reliablePackets.removeFromClient(ackID);
}*/

void SubspaceNetworkHandler::encryptPacket(SubspacePacket& p)
{
	if(!encrypt)
		return;
	
	int start = 1;
	Uint32 encrypted = key;
	
	if(p.getByte(0) == 0)
		start++;

	for(Uint i=start; i < p.getSize(); i += 4)
	{
		encrypted ^= *(Uint32 *)(keyStream + (i - start)) ^ p.getDword(i);
		p.setDword(encrypted, i);
	}
}

void SubspaceNetworkHandler::decryptPacket(SubspacePacket &p)
{	
	if(!encrypt)
		return;

	Uint32 decrypted = key;
	int start = 1;

	if(p.getByte(0) == 0) //If Byte at Offset 0 is 0x00
		start++;
	
	for(Uint i = start; i < p.getSize(); i += 4)
	{
		Uint32 encrypted = p.getDword(i);
		decrypted ^= *(Uint32 *)(keyStream + (i - start)) ^ encrypted;
		p.setDword(decrypted, i);
		decrypted = encrypted;
	}
}

void SubspaceNetworkHandler::makeStream(Uint32 key)
{
	Sint32 temp = 0;
	
	key = key;

	for(int i = 0; i < MAX_PACKET_SIZE; i += 2) // Each "block" is 2 bytes and the keystream size is 520 bytes
	{
		temp = (Uint32)((Uint64)((Uint64)key * (Uint64)0x834E0B5F) >> 48);
		temp += (temp >> 31);
		key = ((key % 0x1F31D) * 16807) - (temp * 2836) + 123;
		if((Sint32)key < 0)
			key += 0x7FFFFFFF;
		*((Uint16 *)(keyStream + i)) = (Uint16)key;
	}
	encrypt = true;
}