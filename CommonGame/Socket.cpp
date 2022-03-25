#include "Socket.h"

#include <iostream>
using namespace std;

#include "WinUtil.h"

Socket::Socket() : socket_(INVALID_SOCKET)
{
	this->init();
}

bool Socket::init()
{
	WORD version;
	WSADATA stWSAData;

	version = MAKEWORD(2,2);
	if(WSAStartup(version, &stWSAData) == 0)
		return true;
	else 
		return false;
}

bool Socket::close(Uint32 timeout, bool wait)
{
	if(socket_ == INVALID_SOCKET)
		return false;

	printf("Closing socket %d...\n", socket_);
	int result = closesocket(socket_);
	socket_ = INVALID_SOCKET;

	if(result == SOCKET_ERROR)
	{
		printf("Error- Socket.close(): %s\n", getLastWindowsError().c_str());	
		return false;
	}
	else
	{
		printf("Socket closed.\n");	
		return true;
	}
}

bool Socket::initUDP(const string& defaultHost, Uint16 defaultPort)
{
	struct hostent* hostEntry = NULL;
	struct sockaddr_in server;

	socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(socket_ == INVALID_SOCKET)
		return false;

	hostEntry = gethostbyname(defaultHost.c_str());
	if(hostEntry == NULL)
		return false;

	server.sin_family = AF_INET;			//initialize the request server structure
	memcpy((char*)&server.sin_addr, (char*)hostEntry->h_addr, hostEntry->h_length);
	server.sin_port = htons(defaultPort);

	if(connect(socket_, (struct sockaddr*)&server, sizeof(server)) != 0)
		return false;

	return true;
}

bool Socket::sendData(const char* buffer, int bufferSize)
{
	/*printf("buffer send\n");
	for(int i=0; i < bufferSize; i++)
		printf("%x ", buffer[i]);
	printf("\n");*/

	int result = send(socket_, buffer, bufferSize, 0);

	if(result == SOCKET_ERROR)
	{
		cout << "Error: Socket.sendData(): " << getLastWindowsError() << endl;
		return false;
	}
	else 
		return true;
}

bool Socket::receiveData(char* buffer, int bufferSize, int* bytesReceived)
{
	if(socket_ == INVALID_SOCKET)
		return false;

	int result = recv(socket_, buffer, bufferSize, 0);

	/*printf("Buffer received");
	for(int i=0; i < result; i++)
	{
		printf("%x ", buffer[i]);
	}
	printf("\n");*/

	if(result == SOCKET_ERROR || result < 0)
	{
		string error = getLastWindowsError();
		cout << "Error: Socket.receiveData(): " << getLastWindowsError() << endl;

		if(bytesReceived) *bytesReceived = 0;
		return false;
	}
	else
	{
		if(bytesReceived) *bytesReceived = result;
		return true;
	}
}

bool Socket::waitForData(unsigned int timeout)
{
	if(socket_ == INVALID_SOCKET)
		return false;

	timeval waitTime;

	//timeout is in milliseconds

	waitTime.tv_sec = timeout / 1000;
	waitTime.tv_usec = (timeout % 1000) * 1000;

	FD_ZERO(&socketSet_);
	FD_SET(socket_, &socketSet_);

    int result = select(0, &socketSet_, 0, 0, &waitTime);

	if(result == 0)		//time out occured
	{
		return false;
	}
	else if(result > 0)	//data is ready
	{
		return true;
	}
	else		//error occured
	{
		printf("Socket (waitForData) error: %s\n", getLastWindowsError().c_str());
	}

	return false;
}
	