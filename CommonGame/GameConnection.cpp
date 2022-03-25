//David Millman

#include "GameConnection.h"

GameConnection::GameConnection()
{
}

GameConnection::~GameConnection()
{

}


bool GameConnection::connect(const string& host, Uint16 port)
{
	if(!socket.initUDP(host, port))
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
	this->debugLog("Started connection for %s, %s:%d", buf, buf2, port);

	return true;
}

bool GameConnection::disconnect()
{
	if(!socket.close())
	{
		this->debugLog("Failed to close socket\n");
		return false;
	}

	return true;
}

/*bool GameConnection::sendObject(Serializable& object)
{
	return sendString(object.serialize());
}

bool GameConnection::receiveObject(Serializable* object)
{
	static const int bufferSize = 1024;
	char buffer[bufferSize];
	int bytesReceived = 0;
    string s;

	if(!object)
		return false;

	do
	{
		socket.receiveData(buffer, bufferSize, &bytesReceived);
		s.append(buffer);
	}
	while(bytesReceived == bufferSize);

	object->deserialize(s);

	return true;
}

bool GameConnection::sendString(const string& s)
{
	return socket.sendData(s.c_str(), static_cast<int>(s.size()) );
}*/
