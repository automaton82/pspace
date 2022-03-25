#include "SubspaceCorePackets.h"

#include "DataTypes.h"

/*struct EncryptionRequest
{
	// Offset	Length	Descriptoin
	// 0		2		Type Byte 0x00,0x01
	// 2		4		Client Encryption Key
	// 6		2		Client Version/Type
	
	EncryptionRequest(DWord encryptionKey) : 
		type(0x01),
		encKey(encryptionKey),
		version(6)
	{}

	Word type;
	DWord encryptionKey;
	Word version;
};*/