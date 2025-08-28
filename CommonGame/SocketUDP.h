//David Millman

#ifndef _SOCKETUDP_H_
#define _SOCKETUDP_H_

#include <string>
using std::string;

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

#include "DataTypes.h"

class SocketUDP
{
public:
	SocketUDP();

	// Accessors
	string getHost() const;
	string getIPAddress() const;
	Uint16 getPort() const;

	///////////////////////////

	bool close();
	bool open(const string& defaultHost, Uint16 defaultPort);

	bool receiveData(char* buffer, int bufferSize, int* bytesReceived = 0);
	bool sendData(const char* buffer, int bufferSize);
	bool waitForData(unsigned int timeout);	//assumes timeout in milliseconds
    
private:

	string getWindowsSocketError(int errorCode);
	bool windowsSocketInit();	//multiple calls to WSAStartup are ok, windows handles that
	bool windowsSocketCleanup();

private:

	bool isWindowsSocketInit_;

	string host_;
	string ipaddr_;
	Uint16 port_;

	SOCKET socket_;
	fd_set socketSet_;
};

#endif