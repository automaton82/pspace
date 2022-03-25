#include "SubspaceGameProtocol.h"

#include "Timer.h"

#include "SubspaceChatCommandReceiver.h"
#include "SubspaceClientCommandListener.h"

#include "SubspacePacketFactory.h"
using namespace SubspacePacketFactory;

#include "SubspacePacketInterpreter.h"
using namespace SubspacePacketInterpreter;

SubspaceGameProtocol::SubspaceGameProtocol() : 
	packetHandlerThread_(static_cast<Thread::startRoutine>(packetHandlerRoutine)),
	connected_(false),
	s2cSlowTotal_(0), s2cFastTotal_(0),
	s2cSlowCurrent_(0), s2cFastCurrent_(0),
	s2cWeaponCount_(0)
{
	isRunning_ = true;
}

SubspaceGameProtocol::~SubspaceGameProtocol()
{
	disconnect();
}

bool SubspaceGameProtocol::connect(const string& user, const string& password, const string& host, Uint16 port, Uint timeout)
{
	Timer timer, timer2;
	timer.start();

	packetHandlerThread_.start(this);
	coreProtocol_.connect(host, port);

	while(!coreProtocol_.isConnected() && timer.getElapsedTime() < timeout)
	{
		Sleep(1);
	}

	if(coreProtocol_.isConnected())
	{
		connected_ = true;
	}
	else
	{
		connected_ = false;
		isRunning_ = false;
		packetHandlerThread_.stop();
		coreProtocol_.disconnect();
	}

	return connected_;
}
	
bool SubspaceGameProtocol::connect2(const string& host, Uint16 port, Uint timeout)
{
	Timer timer, timer2;
	SubspacePacket packet;

	timer.start();

	if(!coreProtocol_.connect2(host, port))
		return false;
		
	packetHandlerThread_.start(this);

	return true;
	/*doServerPassword(user, password, false);
	coreProtocol_.sendSyncRequest();
	
	while(!this->isConnected() && timer.getElapsedTime() < timeout)
		Sleep(1);
	
	if(this->isConnected())
	{
		printf("Login successful.\n");

		connected_ = true;
		return true;
	}
	else
	{
		printf("Login timed out.\n");

		connected_ = false;
		isRunning_ = false;
		packetHandlerThread_.stop();
		coreProtocol_.disconnect();
		return false;
	}*/
}

bool SubspaceGameProtocol::disconnect()
{
	bool result = false;

	isRunning_ = false;

	if(isConnected())
	{
		coreProtocol_.sendDisconnect();

		result = coreProtocol_.disconnect();
    
		packetHandlerThread_.stop();//waitForExit();//

		connected_ = false;
	}

	//return coreProtocol_.disconnect();
	return result;
}

bool SubspaceGameProtocol::isConnected() const
{
	return connected_;
}

bool SubspaceGameProtocol::isRunning() const
{
	return isRunning_;
}

void SubspaceGameProtocol::setStartShip(ShipType ship)
{
	startShip_ = ship;
}

void SubspaceGameProtocol::setStartArena(const string& arenaName)
{
	startArena_ = arenaName;
}

/*Uint32 SubspaceGameProtocol::getServerTimeStamp() const
{
	return serverTimeStamp;
}*/

void SubspaceGameProtocol::executeInboundCommands()
{
	while(clientCommands_.size() > 0)
	{
		SubspaceClientCommand* clientCmd = clientCommands_.front();
		clientCommands_.pop_front();

		this->CommandGenerator<SubspaceClientCommandListener>::invokeCommand(clientCmd);

		assert(clientCmd);
		delete clientCmd;
		clientCmd = 0;
	}

	while(chatCommandsInbound_.size() > 0)
	{
		SubspaceChatCommand* chatCmd = chatCommandsInbound_.front();
		chatCommandsInbound_.pop_front();

		this->CommandGenerator<SubspaceChatCommandReceiver>::invokeCommand(chatCmd);

		assert(chatCmd);
		delete chatCmd;
	}
}

void SubspaceGameProtocol::queueClientCommand(SubspaceClientCommand* cmd)
{
	// do mutex locking?

	clientCommands_.push_back(cmd);

	// unlock
}

void SubspaceGameProtocol::queueServerCommand(SubspaceServerCommand* cmd)
{
	// do mutex locking?

	serverCommands_.push_back(cmd);

	// unlock
}

void SubspaceGameProtocol::queueChatCommandInbound(SubspaceChatCommand* cmd)
{
	// do mutex locking?
	
	chatCommandsInbound_.push_back(cmd);

	// unlock
}

void SubspaceGameProtocol::queueChatCommandOutbound(SubspaceChatCommand* cmd)
{
	chatCommandsOutbound_.push_back(cmd);
}

void packetHandlerRoutine(void* arg)
{
	SubspaceGameProtocol* mainProtocol = static_cast<SubspaceGameProtocol*>(arg);
	
	if(!mainProtocol)
		return;

	SubspacePacket p;
	//mainProtocol->debugLog("Packet handler started");
	while(mainProtocol->isRunning())
	{	
		if(!mainProtocol->coreProtocol_.receivePacket(p))
		{
			//mainProtocol->debugLog("PACKET: Packet receive failed (SubspaceProtocol - packetHandlerRoutine)!");
			Sleep(1);
		}
		else
		{
			mainProtocol->onPacket(p);
		}
	}
	//mainProtocol->log("Packet handler stopped.");

	ExitThread(0);
	//return;
}
