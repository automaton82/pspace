//David Millman

#include "SubspaceConnection.h"

#include "Timer.h"

SubspaceConnection::SubspaceConnection()
{
	this->setHeader("Millspace");
	/*this->mainProtocol.setLog(fopen("mainLog.txt","w"));
	this->coreProtocol.setLog(fopen("coreLog.txt","w"));
	this->netHandler.setLog(fopen("packetLog.txt","w"));*/
}

SubspaceConnection::~SubspaceConnection()
{
}

bool SubspaceConnection::connect(const string& host, Uint16 port, Uint timeout)
{
	Timer timer;
	timer.start();

	while(!mainProtocol_.isConnected() && timer.getElapsedTime() < timeout)
	{
		Sleep(50);
	}

	if(mainProtocol_.isConnected())
		return true;
	else
		return false;
}

bool SubspaceConnection::disconnect()
{
	return mainProtocol_.disconnect();
}

void SubspaceConnection::setLog(FILE* file)
{
	this->mainProtocol_.setLog(file);
}

void SubspaceConnection::setHeader(const string& header)
{
	this->mainProtocol_.setHeader(header + " [Main]");
}
