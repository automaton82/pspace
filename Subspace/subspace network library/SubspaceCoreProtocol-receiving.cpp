#include "SubspaceCoreProtocol.h"

#include "SubspacePacketFactory.h"
using namespace SubspacePacketFactory;

#include "SubspacePacketInterpreter.h"
using namespace SubspacePacketInterpreter;

#include "PacketLog.h"

bool SubspaceCoreProtocol::receivePacket(SubspacePacket& p)
{
	while(inboundPackets_.size() == 0 && isRunning())
	{
		SubspacePacket temp;
		if(!netHandler_.receivePacket(temp))	//decrypt packet
			return false;

		onPacket(temp);
	}
	
	if(inboundPackets_.size() > 0)
	{
		p = inboundPackets_.front();
		inboundPackets_.pop_front();
		return true;
	}

	return false;
}

void SubspaceCoreProtocol::onPacket(SubspacePacket& p)
{
	PacketLog::log("->Core: 0x%02x%02x: \n", p.getByte(0), p.getByte(1));
	
	if(p.getByte(0) == 0x00)			//handled by the core protocol
	{
		switch(p.getByte(1))
		{
		case SSCPT_EncryptionResponse:
			onEncryptionResponse(p);
			break;
		case SSCPT_Reliable:
			onReliable(p);
			break;		
		case SSCPT_ReliableAck:
			onReliableAck(p);
			break;
		case SSCPT_SyncRequest:
			onSyncRequest(p);
			break;
		case SSCPT_SyncResponse:
			onSyncResponse(p);
			break;
		case SSCPT_Disconnect: 
			onDisconnect(p);
			break;
		case SSCPT_SmallChunk:
			onSmallChunk(p);
			break;
		case SSCPT_SmallChunkTail:
			onSmallChunkTail(p);
			break;
		case SSCPT_FileTransfer:
			onFileTransfer(p);
			break;
		case SSCPT_Cluster:
			onCluster(p);
			break;
		default:
			//receivedPackets_.push_back(p);
			printf("UNKNOWN packet type, size: %d\n", p.getSize());
		}
	}
	else
	{
		PacketLog::log("->queue: 0x%02x\n", p.getByte(0));
		inboundPackets_.push_back(p);
	}
}


//0x0002
bool SubspaceCoreProtocol::onEncryptionResponse(SubspacePacket& p)
{
	//printf("PACKET: Server Encryption Response, size: %d\n", p.getSize());
	
	Uint32 key;
	encryptionResponse(p, &key);
	netHandler_.setEncryptionKey(key);

	if(key == sentKey_)						
		netHandler_.setEncryptionStatus(false);

	reliableResend_ = true;

	PacketLog::log("->Core: Encryption Key: %08x\n", key);
	/*makeStream(key);*/

	isConnected_ = true;
	//this->receivedPackets_.push_back(p);
	
	//mainProtocol->sendPassword()
	//sendSyncRequest();
	
	return true;
}

//0x0003
bool SubspaceCoreProtocol::onReliable(SubspacePacket& p)
{
	//printf("PACKET: Reliable Packet Received\n");

	Uint32 ackID;
	SubspacePacket body;
	
	//p.print("Before");
	reliablePacket(p, &ackID, &body);
	PacketLog::log("->Core: Reliable[%d]\n", ackID);
	//body.print("After");

	sendReliableAck(ackID);
	
	reliablePackets_.addFromServer(ackID, body);

	bool retval = false;
	while(reliablePackets_.availableFromServer())
	{
		SubspacePacket next;
		int id = reliablePackets_.getNextFromServer(next);
		
		PacketLog::log("->Core: Reliable[%d], rehandling-\n\t", id);
		PacketLog::tab();
		
		onPacket(next);

		PacketLog::untab();
		retval = true;
	}
	
	return retval;
}
			
//0x0004
bool SubspaceCoreProtocol::onReliableAck(SubspacePacket& p)
{
	Uint32 ackID;
	SubspacePacket body;

	reliablePacket(p, &ackID, &body);
	PacketLog::log("->Core: Reliable Ack[%d], size: %d\n", ackID, body.getSize());

	reliablePackets_.removeFromClient(ackID);	//stop resending packet
	
	return true;	
}

//0x0005
bool SubspaceCoreProtocol::onSyncRequest(SubspacePacket& p)
{
	PacketLog::log("->Core: Sync Request, size: %d\n", p.getSize());

	Uint32 newTimeStamp;
	syncRequest(p, &newTimeStamp, NULL, NULL);
	
	setServerTimeStamp(newTimeStamp);

	sendSyncResponse();

	return true;
}

//0x0006
bool SubspaceCoreProtocol::onSyncResponse(SubspacePacket& p)
{
	PacketLog::log("->Core: Sync Response, size: %d\n", p.getSize());

	Uint32 pingTime, pongTime;

	syncResponse(p, &pingTime, &pongTime);

	setServerTimeStamp(pongTime);

	pingCurrent_ = GetTickCount()/10 - (int)pingTime;		//TODO: factor this out

	if(pingCurrent_ > pingHigh_) 
		pingHigh_ = pingCurrent_;

	if(!pingLow_ || pingCurrent_ < pingLow_)
		pingLow_ = pingCurrent_;

	// Average Ping = (3 * OldAverage + CurrentPing) / 4

	if(!pingAverage_)
		pingAverage_ = pingCurrent_;

	pingAverage_ = (pingAverage_*3 + pingCurrent_) / 4;		//TODO: factor this better

	return true;
}

//0x0007
bool SubspaceCoreProtocol::onDisconnect(SubspacePacket& p)
{
	//this->log("Disconnected by server");

	PacketLog::log("->Core: Disconnect.\n");

	this->disconnect();

	//exit(0);
	return true;
}

//0x0008
bool SubspaceCoreProtocol::onSmallChunk(SubspacePacket& p)
{
	PacketLog::log("->Core: Small Chunk Received, size: %d\n", p.getSize());

	SubspacePacket temp;
	smallChunkTail(p, &temp);
	dataBuffer_.append(temp);

	PacketLog::log("\tData buffer size: %d\n", dataBuffer_.getSize());
	return true;
}

//0x0009
bool SubspaceCoreProtocol::onSmallChunkTail(SubspacePacket& p)
{
	onSmallChunk(p);
	PacketLog::log("->Core: Small Chunk Tail Received: %d\n", p.getSize());

	PacketLog::log("->Core: Handling Chunk-\n");
	PacketLog::tab();
	
	onPacket(dataBuffer_);

	PacketLog::untab();

	dataBuffer_.clear();
	dataBuffer_.resize(0);
	return true;
}

//0x000A
bool SubspaceCoreProtocol::onFileTransfer(SubspacePacket& p)
{
	string buffer;
	Uint32 fileSize;
	//size_t fileBufferSize = fileBuffer_.size();

	fileTransfer(p, &fileSize, &buffer);
	size_t bufferSize = buffer.size();

	if(bufferSize != 480)
	{
		printf("END OF FILE\n");
	}
	fileBuffer_.append(buffer);

	printf("Total file size: %d, file buffer size: %d, buffer size: %d\n", fileSize, fileBuffer_.size(), buffer.size());
	//this->log("File download [%d]: %.2f%% complete", fileSize, (double)fileBuffer.size()/(double)fileSize * 100);

	if(fileBuffer_.size() >= fileSize)
	{
		SubspacePacket temp((Uint)(fileBuffer_.size()));
		temp.setString(fileBuffer_, 0);
		onPacket(temp);

		fileBuffer_.clear();
		fileBuffer_.resize(0);
	}

	return true;
}

//0x000E
bool SubspaceCoreProtocol::onCluster(SubspacePacket& p)
{
	SubspacePacket subPacket;
	Uint offset = 2; //Step ahead of the Header Info
	Uint size = 0;
	Uint packets = 0;

	PacketLog::log("->Core: Cluster Packet Start\n");
	PacketLog::log("------------------------------\n");

	Uint total = p.getSize();
	while(offset < total) //Loop Until Pos is at End of Packet
	{
		PacketLog::log("%d:", packets);
		PacketLog::tab();

		size = p.getByte(offset); //Get Len of Sub Packet

		subPacket = p.subStr(offset+1, offset+1+size); //Put Sub Packet in Temp Packet
		/*subPacket.clear();
		subPacket.setString(p.getString(offset+1, size), 0);*/
	
		onPacket(subPacket); //Dispatch Temp Packet
		offset += size+1; //Increase Pos upto Next Sub Packet
		packets++;

		PacketLog::untab();
	}
	PacketLog::log("------------------------------\n");
	return true;
}