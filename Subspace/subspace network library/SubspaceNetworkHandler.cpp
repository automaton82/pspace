#include "SubspaceNetworkHandler.h"

#include <iostream>
using namespace std;

#include "Checksum.h"
#include "MathUtil.H"
#include "WinUtil.h"

const double SubspaceNetworkHandler::defaultFailRate = 20.0;

SubspaceNetworkHandler::SubspaceNetworkHandler() :
	doEncrypt_(false),
	key(0),

	bytesReceived_(0),
	bytesSent_(0),
	packetsSent_(0),
	packetsReceived_(0),

	connectionTest_(false),
	timeout_(defaultTimeout)
{
}

SubspaceNetworkHandler::~SubspaceNetworkHandler()
{
	//disconnect();
}

Uint SubspaceNetworkHandler::getBytesSent() const
{
	return bytesSent_;
}

Uint SubspaceNetworkHandler::getBytesReceived() const
{
	return bytesReceived_;
}

Uint32 SubspaceNetworkHandler::getEncryptionKey() const
{
	return key;
}

bool SubspaceNetworkHandler::getEncryptionStatus() const
{
	return doEncrypt_;
}

Uint SubspaceNetworkHandler::getPacketsReceived() const
{
	return packetsReceived_;
}

Uint SubspaceNetworkHandler::getPacketsSent() const
{
	return packetsSent_;
}



void SubspaceNetworkHandler::setEncryptionKey(Uint32 k)
{
	key = k;
	makeStream(key);
}

void SubspaceNetworkHandler::setEncryptionStatus(bool doEncrypt)
{
	doEncrypt_ = doEncrypt;
}

void SubspaceNetworkHandler::setConnectionTest(bool test)
{
	connectionTest_ = test;
}

bool SubspaceNetworkHandler::connect(const string& host, Uint16 port)
{
	if(!socket_.open(host, port))
	{
		cerr << "UDP startup failed." << endl;
		return false;
	}

	cout << "Connecting to " << socket_.getHost() << " (" << socket_.getIPAddress() << ":" << socket_.getPort() << ")" << endl;

	return true;
}

bool SubspaceNetworkHandler::disconnect()
{
	return socket_.close();
}


bool SubspaceNetworkHandler::receivePacket(SubspacePacket& p)
{
	char data[MAX_PACKET_SIZE];
	int bytes;

	//p.data = new Uint8[MAX_PACKET_SIZE];

	//wait for a new packet - blocking operation
	if(!socket_.waitForData(timeout_))
	{
		cout << "No data coming from server... (" << timeout_ << " ms since last packet)" << endl;
		cout << "RECEIVEPACKET: " << getLastWindowsError() << endl;
		return false;
	}
	//if(!socket_.receiveData(reinterpret_cast<char*>(p.data), MAX_PACKET_SIZE, &p.dataSize))
	if(!socket_.receiveData(data, MAX_PACKET_SIZE, &bytes))
	{
		return false;
	}

	//build packet

	p.resize(bytes);
	p.setData(data, bytes, 0);

	packetsReceived_++;
	bytesReceived_ += p.getSize();

	if(doEncrypt_)
		decryptPacket(p);

	if(p.getSize() < 0)
		return false;

	return true;
}


bool SubspaceNetworkHandler::sendPacket(SubspacePacket& p)
{
	if(p.size() > MAX_PACKET_SIZE)
		return sendPacketHelper(p);		//recursively break up packets

	++packetsSent_;
	bytesSent_ += p.getSize();

	if(doEncrypt_)
		encryptPacket(p);
	
	if(!connectionTest_ || MathUtil::unitRand() < failRate_)			//if testing connection, fail 10% of packets
	{
		if(!socket_.sendData(reinterpret_cast<const char*>(p.data), p.size()))
			return false;
	}
	
	return true;
}

bool SubspaceNetworkHandler::sendPacketHelper(SubspacePacket& p)
{
	const int chunkDataSize = 518;

	if(p.size() <= MAX_PACKET_SIZE)
		return sendPacket(p);

	SubspacePacket chunk;
	Uint offset = 0;
	while(offset < p.size())
	{
		int size = min(chunkDataSize, p.size() - offset);	//remaining part of the packet or a maxsize packet

		//if(p.size() - offset < MAX_PACKET_SIZE)
		if(p.size() < MAX_PACKET_SIZE + offset)				//check if this is the last piece of the packet
		{
			chunk.data = &p.data[offset];
			chunk.dataSize = size;
			//chunk = packetFactory.smallChunkTail(p.getString(offset, size));
		}
		else
		{
			chunk.data = &p.data[offset];
			chunk.dataSize = size;
			//chunk = packetFactory.smallChunk(p.getString(offset, size));
		}

		sendPacket(chunk);
        
		offset += chunkDataSize;		//should increase by size? - ok, if size < chunkDataSize then the loop stops anyway
	}
	chunk.data = 0;		//make sure the packet isn't auto deleted

	return true;
}


void SubspaceNetworkHandler::decryptPacket(SubspacePacket &p)
{	
	if(!doEncrypt_)
		return;

	Uint32 decrypted = key;
	int start = 1;

	if(p.getByte(0) == 0) //If Byte at Offset 0 is 0x00
		start++;
	
	for(Uint i = start; i < p.getSize(); i += 4)		//TODO: who wrote this?
	{
		Uint32 encrypted = p.getDword(i);
		decrypted ^= *(Uint32 *)(keyStream + (i - start)) ^ encrypted;
		p.setDword(decrypted, i);
		decrypted = encrypted;
	}
}

void SubspaceNetworkHandler::encryptPacket(SubspacePacket& p)
{
	if(!doEncrypt_)
		return;
	
	int start = 1;
	Uint32 encrypted = key;
	
	if(p.getByte(0) == 0)
		start++;

	for(Uint i=start; i < p.getSize(); i += 4)			//TODO: who wrote this?
	{
		encrypted ^= *(Uint32 *)(keyStream + (i - start)) ^ p.getDword(i);
		p.setDword(encrypted, i);
	}
}


void SubspaceNetworkHandler::makeStream(Uint32 key)
{
	//TODO: who wrote this too?

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
	doEncrypt_ = true;
}