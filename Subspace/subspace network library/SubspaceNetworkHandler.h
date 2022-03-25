#ifndef _SUBSPACENETWORKHANDLER_H_
#define _SUBSPACENETWORKHANDLER_H_

#include "SocketUDP.h"

#include "SubspacePacket.h"
#include "SubspaceCorePackets.h"

class SubspaceNetworkHandler
{
public:

	SubspaceNetworkHandler();
	~SubspaceNetworkHandler();

	//Accessors
	Uint32 getEncryptionKey() const;
	bool getEncryptionStatus() const;
	Uint getBytesSent() const;
	Uint getBytesReceived() const;
	Uint getPacketsSent() const;
	Uint getPacketsReceived() const;

	//Mutators
	void setConnectionTest(bool test);
	void setEncryptionStatus(bool doEncrypt);
	void setEncryptionKey(Uint32 key);
	void setTimeout(Uint timeout);

	//Network functionality
	bool connect(const string& host, Uint16 port);
	bool disconnect();
	bool receivePacket(SubspacePacket& p);		//this is a blocking operation
	bool sendPacket(SubspacePacket& p);
	bool sendPacketHelper(SubspacePacket& p);

private:
	
	//TODO: who wrote the encryption - how does it work?
	void decryptPacket(SubspacePacket& p);
	void encryptPacket(SubspacePacket& p);
	void makeStream(Uint32 key);

	static const int defaultTimeout = 10000;
	static const double defaultFailRate;

private:
	
	//Network Functionality
	SocketUDP socket_;
	bool connectionTest_;			//if this is on, 10% of packets will fail to be delivered or received
	double failRate_;
	Uint timeout_;	
	
	//Network info
	Uint bytesReceived_;
	Uint bytesSent_;
	Uint packetsReceived_;
	Uint packetsSent_;

	//Encryption
	bool doEncrypt_;
	Uint32 key;
	Uint8 keyStream[520];
};

#endif