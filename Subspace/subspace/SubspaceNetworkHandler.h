//David Millman

#ifndef _SUBSPACENETWORKHANDLER_H_
#define _SUBSPACENETWORKHANDLER_H_

#ifdef _WIN32
#include <windows.h>
#else
// Linux compatibility
typedef unsigned char BYTE;
typedef unsigned short WORD;  
typedef unsigned int DWORD;
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#endif

#include <vector>
#include <map>
#include <iostream>

#include "GameConnection.h"
#include "Socket.h"
#include "Thread.h"
#include "FileLogger.h"
#include "Functor.h"

#include "Checksum.h"
#include "../subspace network library/SubspacePacket.h"
#include "SubspaceReliableManager.h"
#include "SubspacePacketFactory.h"
#include "SubspacePacketInterpreter.h"

/**********************************
* This will need to be threadsafe *
***********************************/


class SubspaceNetworkHandler
	: public FileLogger
{
public:

	SubspaceNetworkHandler();
	~SubspaceNetworkHandler();

	bool connect(const string& host, Uint16 port);
	bool disconnect();

	/**************
	 * Encryption *
	 **************/
	
	void setEncryptionStatus(bool doEncrypt);
	void setEncryptionKey(Uint32 key);

	bool getEncryptionStatus();
	Uint32 getEncryptionKey();

	/****************
	 * Traffic Info *
	 ****************/
	Uint getPacketsSent() const;
	Uint getBytesSent() const;
	Uint getPacketsReceived() const;
	Uint getBytesReceived() const;

	/****************************
	 * Connection Functionality *
	 ****************************/
	
	void setTimeout(Uint timeout);
	void setConnectionTest(bool test);
	bool sendPacket(SubspacePacket& p);
	bool sendPacketHelper(SubspacePacket& p);
	
	bool receivePacket(SubspacePacket& p);

private:
	//friend void reliablePacketHandlerRoutine(void* ssNetworkHandler);

	void encryptPacket(SubspacePacket& p);
	void decryptPacket(SubspacePacket& p);
	void makeStream(Uint32 key);

	Socket socket_;

	bool connectionTest_;
	
	Uint packetsSent;
	Uint bytesSent;
	Uint packetsReceived;
	Uint bytesReceived;

	bool encrypt;
	Uint32 key;
	Uint8 keyStream[520];

	Uint timeout_;
};

#endif