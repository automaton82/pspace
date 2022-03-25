#ifndef _SUBSPACECOREPACKETS_H_
#define _SUBSPACECOREPACKETS_H_

enum SubspaceCorePacketType
{
	SSCPT_Unknown	= 0x0000,

	SSCPT_EncryptionRequest		= 0x0001,
	SSCPT_EncryptionResponse	= 0x0002,
	SSCPT_Reliable			= 0x0003,
	SSCPT_ReliableAck		= 0x0004,
	SSCPT_SyncRequest		= 0x0005,
	SSCPT_SyncResponse		= 0x0006,
	SSCPT_Disconnect		= 0x0007,
	SSCPT_SmallChunk		= 0x0008,
	SSCPT_SmallChunkTail	= 0x0009,
	SSCPT_FileTransfer		= 0x000A,
	SSCPT_Cluster			= 0x000E,

	SSCPT_NumTypes
};

//struct EncryptionRequest;

#endif