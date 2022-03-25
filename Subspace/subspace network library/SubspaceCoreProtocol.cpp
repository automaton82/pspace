#include "SubspaceCoreProtocol.h"

#include "SubspacePacketFactory.h"
using namespace SubspacePacketFactory;

#include "SubspacePacketInterpreter.h"
using namespace SubspacePacketInterpreter;

SubspaceCoreProtocol::SubspaceCoreProtocol() :
	reliablePacketThread_(static_cast<Thread::startRoutine>(reliablePacketHandlerRoutine)),
	isConnected_(false),

	reliableResend_(false),
	resendDelay_(0),

    pingCurrent_(0), pingHigh_(0), pingLow_(0), pingAverage_(0), 
	isRunning_(true)
{
}

SubspaceCoreProtocol::~SubspaceCoreProtocol()
{
	isRunning_ = false;
	disconnect();
}


bool SubspaceCoreProtocol::connect(const string& host, Uint16 port)
{
	if(!netHandler_.connect(host, port))	
		return false;

	isConnected_ = true;

	reliablePacketThread_.start(this);
    return sendEncryptionRequest();
}

bool SubspaceCoreProtocol::connect2(const string& host, Uint16 port)
{
	SubspacePacket packet;

	if(!netHandler_.connect(host, port))	
		return false;

	sendEncryptionRequest();

	if(!netHandler_.receivePacket(packet))
		return false;
	if(packet.getByte(0) != 0 || packet.getByte(1) != SSCPT_SyncRequest)
	{
		printf("Invalid login packet sequence.\n");
		return false;
	}
	else
	{
		onSyncRequest(packet);
	}

	if(!netHandler_.receivePacket(packet))
		return false;
	if(packet.getByte(0) != 0 || packet.getByte(1) != SSCPT_EncryptionResponse)
	{
		printf("Invalid login packet sequence.\n");
		return false;
	}
	else
	{
		onEncryptionResponse(packet);
	}
	
	isConnected_ = true;
	reliablePacketThread_.start(this);

    return true;
}

bool SubspaceCoreProtocol::disconnect()
{
	isRunning_ = false;

	if(isConnected_)
	{
		reliablePacketThread_.stop();//waitForExit();//
		reliablePackets_.clear();
		isConnected_ = false;
	}

	return netHandler_.disconnect();
}

bool SubspaceCoreProtocol::isConnected() const
{
	return isConnected_;
}

bool SubspaceCoreProtocol::isRunning() const
{
	return isRunning_;
}

Uint SubspaceCoreProtocol::getPacketsReceived() const
{
	return netHandler_.getPacketsSent();
}

Uint SubspaceCoreProtocol::getPingCurrent() const
{
	return pingCurrent_;
}

Uint SubspaceCoreProtocol::getPingLow() const
{
	return pingLow_;
}

Uint SubspaceCoreProtocol::getPingHigh() const
{
	return pingHigh_;
}

Uint SubspaceCoreProtocol::getPingAverage() const
{
	return pingAverage_;
}

Uint SubspaceCoreProtocol::getReliableSentPackets() const
{
	return reliablePackets_.getLastClientID();
}

Uint SubspaceCoreProtocol::getReliableReceivedPackets() const
{
	return reliablePackets_.getLastServerID();
}

Uint32 SubspaceCoreProtocol::getServerTimeStamp()
{
    return serverTimeStamp_ + GetTickCount() - clientTimeStamp_; //timestamp difference
	//return serverTimeStamp_;
}

void SubspaceCoreProtocol::setServerTimeStamp(Uint32 timestamp)
{
	//this->log("Timestamp old,new: %d, %d", serverTimeStamp_, timestamp);
	
	serverTimeStamp_ = timestamp;
	clientTimeStamp_ = GetTickCount();
}


void reliablePacketHandlerRoutine(void* arg)
{
	int defaultResendDelay = 400;

	//TODO: finish this - huh?
	SubspaceCoreProtocol* coreProtocol = (SubspaceCoreProtocol*)(arg);
		
	//coreProtocol->log("Reliable handler started.");
	while(coreProtocol->isRunning())
	{
		if(!coreProtocol->resendDelay_)
			coreProtocol->resendDelay_ = defaultResendDelay;

		while(coreProtocol->reliablePackets_.availableFromClient(GetTickCount()/10, coreProtocol->resendDelay_))
		{
			SubspacePacket resendPacket = coreProtocol->reliablePackets_.getNextFromClient(GetTickCount() / 10, coreProtocol->resendDelay_);
			coreProtocol->reliablePackets_.addFromClient(resendPacket);

			printf("Reliable packet resent: Client[%d]  Server[%d]\n", coreProtocol->reliablePackets_.getLastClientID(), coreProtocol->reliablePackets_.getLastServerID());
			coreProtocol->sendPacket(resendPacket);
		}
		Sleep(coreProtocol->resendDelay_);
	}
	//coreProtocol->log("Reliable handler stopped.");

	ExitThread(0);
}