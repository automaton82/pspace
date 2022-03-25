//Adapted from Subspace Snrrrubs

#ifndef _SUBSPACEPACKET_H_
#define _SUBSPACEPACKET_H_

#include <string>
using std::string;

#include "DataTypes.h"

static const int MAX_PACKET_SIZE = 520;

//NOTE: this is incorrect as a general SubspacePacket structure, 
//subspace packets are stored in little-endian byte order

class SubspacePacket  
	//public Serializable
{
public:

	SubspacePacket();
	SubspacePacket(Uint size);
	SubspacePacket(const SubspacePacket& copy);
	virtual ~SubspacePacket();

	// Operators
	//check this operator
	SubspacePacket& operator =(const SubspacePacket& rhs);

	Uint8& operator[] (Uint offset);
	const Uint8& operator[] (Uint offset) const;

	// Accessors
	Uint8 getByte(Uint offset) const;
	Uint16 getWord(Uint offset) const;
	Uint32 getDword(Uint offset) const;
	string getString(Uint offset, Uint maxLength = 0) const;
	void getData(void* buffer, Uint bufferSize, Uint offset = 0) const;
	
	// Mutators
	void setByte(Uint8 value, Uint offset);
	void setWord(Uint16 value, Uint offset);
	void setDword(Uint32 value, Uint offset);
	void setString(const string& value, Uint offset, Uint maxLength = 0);
	void setData(void* buffer, Uint bufferSize, Uint offset = 0);

	Uint getSize() const;
	Uint size() const;

	void print(const char* name = "SubspacePacket");

	// Utility
	SubspacePacket subStr(Uint start, Uint end = 0) const;

	void append(const SubspacePacket& p);
	void prepend(const SubspacePacket& p);

	void resize(Uint size);
	void clear();	

	string serialize() const;
	void deserialize(const string& s);

private:

	static unsigned long letol(char* data);	//little endian to long
	static unsigned short letos(char* data);	//little endian to short

	void stole(unsigned short, char* data);		// short to little endian
	void ltole(unsigned long, char* data);		// long to little endian

public:
	Uint8* data;
	int dataSize;
	//std::basic_string<Uint8> SubspacePacket;
};

#endif
