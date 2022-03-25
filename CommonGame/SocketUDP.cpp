#include "SocketUDP.h"

#include <iostream>
using namespace std;

#include "WinUtil.h"

SocketUDP::SocketUDP() : 
	isWindowsSocketInit_(false),
	socket_(INVALID_SOCKET)
{
}

SocketUDP::~SocketUDP()
{
	windowsSocketCleanup();
}

string SocketUDP::getHost() const
{
	return host_;
}

string SocketUDP::getIPAddress() const
{
	return ipaddr_;
}

Uint16 SocketUDP::getPort() const
{
	return port_;
}

bool SocketUDP::close()
{
	if(socket_ == INVALID_SOCKET)
		return false;

	int result = closesocket(socket_);
	socket_ = INVALID_SOCKET;

	if(result == SOCKET_ERROR)
	{
		cerr << "Error: Socket.close(): " << getLastWindowsError() << endl;
		return false;
	}
	else
	{
		return true;
	}
}

bool SocketUDP::open(const string& defaultHost, Uint16 defaultPort)
{
	if(!isWindowsSocketInit_)		//do global windows initialization
	{		
		if(!windowsSocketInit())
		{
			cerr << "Error: SocketUDP.windowsSocket(): " << getLastWindowsError() << endl;
			return false;
		}
		isWindowsSocketInit_ = true;
	}

	struct hostent* hostEntry = NULL;
	struct sockaddr_in server;

	//initialize the socket for IPv4, UDP
	socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);		
	if(socket_ == INVALID_SOCKET)
		return false;

	//get the name of host, i.e convert a string (xxx.xxx.xxx.xxx or url.com) to a host entry
	hostEntry = gethostbyname(defaultHost.c_str());			
	if(hostEntry == NULL)
		return false;

	//initialize the request server structure
	server.sin_family = AF_INET;							
	memcpy((char*)&server.sin_addr, (char*)hostEntry->h_addr, hostEntry->h_length);
	server.sin_port = htons(defaultPort);

	if(connect(socket_, (struct sockaddr*)&server, sizeof(server)) != 0)
		return false;

	host_ = defaultHost;
	port_ = defaultPort;
	ipaddr_ = inet_ntoa(server.sin_addr);

	return true;
}

bool SocketUDP::sendData(const char* buffer, int bufferSize)
{
	/*printf("buffer send\n");
	for(int i=0; i < bufferSize; i++)
		printf("%x ", buffer[i]);
	printf("\n");*/

	int result = send(socket_, buffer, bufferSize, 0);

	if(result == SOCKET_ERROR)
		return false;
	else 
		return true;
}

bool SocketUDP::receiveData(char* buffer, int bufferSize, int* bytesReceived)
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
		cout << "Error- Socket.receiveData(): " << getLastWindowsError() << endl;

		if(bytesReceived) *bytesReceived = 0;
		return false;
	}
	else
	{
		if(bytesReceived) *bytesReceived = result;
		return true;
	}
}

bool SocketUDP::waitForData(unsigned int timeout)
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
		cerr << "Error: Socket.waitForData(): " << getLastWindowsError() << endl;
	}

	return false;
}

string SocketUDP::getWindowsSocketError(int errorCode)
{
	string error = "";

	switch(errorCode)
	{
	case WSANOTINITIALISED:	error = "WSANOTINITIALISED";	break;
	case WSAENETDOWN:		error = "WSANETDOWN";			break;
	case WSAEFAULT:			error = "WSAEFAULT";			break;
	case WSAENOTCONN:		error = "WSAENOTCONN";			break;
	case WSAEINTR:			error = "WSAEINTR";				break;
	case WSAEINPROGRESS:	error = "WSAEINPROGRESS";		break;
	case WSAENETRESET:		error = "WSAENETRESET";			break;
	case WSAENOTSOCK:		error = "WSANOTSOCK";			break;
	case WSAEOPNOTSUPP:		error = "WSAEOPNOTSUPP";		break;
	case WSAESHUTDOWN:		error = "WSAESHUTDOWN";			break;
	case WSAEWOULDBLOCK:	error = "WSAEWOULDBLOCK";		break;
	case WSAEMSGSIZE:		error = "WSAEMSGSIZE";			break;
	case WSAEINVAL:			error = "WSAEINVAL";			break;
	case WSAECONNABORTED:	error = "WSAECONNABORTED";		break;
	case WSAETIMEDOUT:		error = "WSAETIMEDOUT";			break;
	case WSAECONNRESET:		error = "WSAECONNRESET";		break;
	default:				error = "UnknownSocketError";	break;
	}

	return error;
}

bool SocketUDP::windowsSocketInit()
{
	WORD version;
	WSADATA stWSAData;

	version = MAKEWORD(2,2);
	return (WSAStartup(version, &stWSAData) == 0);
}

bool SocketUDP::windowsSocketCleanup()
{
	return (WSACleanup() == 0);		
}
