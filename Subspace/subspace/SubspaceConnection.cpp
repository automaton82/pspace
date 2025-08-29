//David Millman

#include "SubspaceConnection.h"

#include "Timer.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

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
#ifdef _WIN32
		Sleep(50);
#else
		usleep(50000);  // 50ms in microseconds
#endif
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
	// TODO: SubspaceCoreProtocol doesn't have setLog method
	// this->mainProtocol_.setLog(file);
}

void SubspaceConnection::setHeader(const string& header)
{
	// TODO: SubspaceCoreProtocol doesn't have setHeader method  
	// this->mainProtocol_.setHeader(header + " [Main]");
}
