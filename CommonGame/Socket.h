//David Millman

#ifndef _SOCKET_H_
#define _SOCKET_H_

#ifdef _WIN32
#include <windows.h>
#include <winsock.h>
#else
// Linux socket compatibility
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
#endif

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