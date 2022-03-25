//David Millman

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <windows.h>
#include <winsock.h>
#include <string>

#include "DataTypes.h"

using std::string;

class Socket
{
public:
	Socket();

	bool init();
	bool initUDP(const string& defaultHost, Uint16 defaultPort);
	bool close(Uint32 timeout = 0, bool wait = false);	//close options not in use right now

	bool sendData(const char* buffer, int bufferSize);
	bool receiveData(char* buffer, int bufferSize, int* bytesReceived = 0);
	bool waitForData(unsigned int timeout);	//assumes timeout in milliseconds
    
private:

	SOCKET socket_;
	fd_set socketSet_;
};

#endif