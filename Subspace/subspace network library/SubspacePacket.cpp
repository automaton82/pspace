#include "SubspacePacket.h"

#include <assert.h>
#include <winsock2.h>

SubspacePacket::SubspacePacket() :
	data(0),
	dataSize(0)
{
}

SubspacePacket::SubspacePacket(Uint s) :
	data(0),
	dataSize(s)
{
	if(dataSize > 0)
	{
		data = new Uint8[dataSize];
		memset(data, 0, dataSize);
	}
}

SubspacePacket::SubspacePacket(const SubspacePacket &copy) : 
	data(0), 
	dataSize(0)
{
	//*this = copy;

	data = new Uint8[copy.dataSize];
	if(data)								//memory allocation actually failed once
	{
		dataSize = copy.dataSize;
		if(size() > 0)
			memcpy(data, copy.data, size());
	}
}

SubspacePacket::~SubspacePacket()
{
	if(data)
	{
		delete [] data;
		data = 0;
		dataSize = 0;
	}
}

SubspacePacket& SubspacePacket::operator =(const SubspacePacket& rhs)
{
	if(this != &rhs)
	{
		this->resize(rhs.size());
		if(size() > 0)
			memcpy(data, rhs.data, size());
	}
	return *this;
}

Uint8& SubspacePacket::operator [](Uint offset)
{
	assert(offset < size());

    /*if(offset >= size())
	{
		static Uint8 bad = 0;
		return bad;
	}*/

	return data[offset];
}

const Uint8& SubspacePacket::operator [](Uint offset) const
{
	assert(offset < size());

	/*if(offset >= size())
	{
		static Uint8 bad = 0;
		return bad;
	}*/

	return data[offset];
}

Uint8 SubspacePacket::getByte(Uint offset) const
{
	//assert(offset <= size()-1);

	return data[offset];
}

Uint16 SubspacePacket::getWord(Uint offset) const
{
	//assert(offset <= size()-2);

	//return letos((char*)&data[offset]);
	Uint32 retVal = 0;
	for(Uint i=0; i < sizeof(Uint16) && offset+i < size(); i++)
		retVal += data[offset+i] << (8 * i);
	return (Uint16)(retVal & 0xFFFF);
}

Uint32 SubspacePacket::getDword(Uint offset) const
{
	//assert(offset <= size()-4);

	//return letol((char*)&data[offset]);
	Uint32 retVal = 0;
	for(Uint i=0; i < sizeof(Uint32) && offset+i < size(); i++)
		retVal += data[offset+i] << (8 * i);
	return retVal;
}


string SubspacePacket::getString(Uint offset, Uint maxLength) const
{
	string ret;
	Uint start = offset;
	Uint end = offset + maxLength;

	if(start > size())
		return ret;

	if(end > size() || !maxLength)
		end = size();

	if(end <= start) return ret;

	ret.resize(end-start, 0);
	for(Uint i=0; i < end-start; ++i)
	{
		ret[i] = data[i + offset];
		//TODO: maybe this isn't legal?
		/*if(data[i + offset])
			ret.at(i) = data[i + offset];
		else
			break;
			//ret.at(i) = ' ';*/
	}

	return ret;
}

void SubspacePacket::setByte(Uint8 value, Uint offset)
{
	//assert(offset <= size()-1);

	data[offset] = value;
}

void SubspacePacket::setWord(Uint16 value, Uint offset)
{
	//assert(offset <= size()-2);

	//stole(value, (char*)&data[offset]);

	for(Uint i=0; i < sizeof(Uint16) && offset+i < size(); i++)
		data[offset+i] = static_cast<Uint8>(((value >> (i * 8)) & 0xFF));
}

void SubspacePacket::setDword(Uint32 value, Uint offset)
{
	//assert(offset <= size()-4);

	//ltole(value, (char*)&data[offset]);

	for(Uint i=0; i < sizeof(Uint32) && offset+i < size(); i++)
		data[offset+i] = static_cast<Uint8>(((value >> (i * 8)) & 0xFF));
}

void SubspacePacket::setString(const string& s, Uint offset, Uint maxLength)
{
	//Uint len = (maxLength == 0 || maxLength > s.length()) ? (Uint)s.length() : maxLength;
	Uint len = 0;
	if(!maxLength || maxLength > s.size())
	{
		len = static_cast<Uint>(s.size());
	}
	else
	{
		len = maxLength;
	}

	Uint i;
	for(i = 0; i < len && offset+i < size(); i++)
		data[offset+i] = s[i];
}

/*Uint8* SubspacePacket::data()
{
	return SubspacePacket;
}

const Uint8* SubspacePacket::data() const
{
	return SubspacePacket;
}*/

Uint SubspacePacket::size() const
{
	return dataSize;
}

void SubspacePacket::resize(Uint s)
{
	Uint8* newData = 0;

	if(s==0)
	{
		dataSize = s;
		//printf("ZERO ALLOCATION\n");
		return;
	}

	if(s <= (Uint)dataSize)
	{
		dataSize = s;
	}
	else
	{
		newData = new Uint8[s];
		//memset(newData, 0, s);

		if(data)
		{
			memcpy(newData, data, dataSize);
			delete [] data;
		}

		dataSize = s;
		
		data = newData;
	}
}

void SubspacePacket::clear()
{
	dataSize = 0;
}

void SubspacePacket::append(const SubspacePacket& p)
{
	int prevSize = size();
	this->resize(size() + p.size());

	for(Uint i=0; i < p.size(); ++i)
	{
		data[prevSize + i] = p[i];
	}
}

void SubspacePacket::prepend(const SubspacePacket& p)
{
	SubspacePacket newPacket(p.size() + size());

	memcpy(newPacket.data, p.data, p.size());
	memcpy(newPacket.data + p.size(), data, size());

	*this = newPacket;
	
	/*Uint8* newData = new Uint8[p.size() + size()];

	memcpy(newData, p.SubspacePacket, p.size());
	memcpy(newData + p.size(), SubspacePacket, size());
	resize(p.size() + size());

	SubspacePacket = newData;*/
	return;
}

string SubspacePacket::serialize() const
{
	return getString(0, 0);
}

void SubspacePacket::deserialize(const string& s)
{
	clear();
	setString(s, 0, 0);
}

void SubspacePacket::print(const char* name)
{
	printf("%s[] = ", name);
	for(Uint i=0; i < size(); ++i)
	{
		printf("[0x%02x] ", getByte(i));
	}
	printf("\n");
}


void SubspacePacket::getData(void* buffer, Uint bufferSize, Uint offset) const
{
	for(Uint i=0; i < bufferSize && offset+i < size(); i++)
		((Uint8*)buffer)[i] = data[offset+i];
	/*for(Uint i=0; i < bufferSize && offset+i < m_sPacket.size(); i++)
		((Uint8*)buffer)[i] = m_sPacket.at(offset+i);*/
}


void SubspacePacket::setData(void* buffer, Uint bufferSize, Uint offset)
{
	for(Uint i=0; i < bufferSize && offset+i < size(); i++)
		data[offset+i] = ((Uint8*)buffer)[i];
	/*for(Uint i=0; i < bufferSize && offset+i < m_sPacket.size(); i++)
		m_sPacket.at(offset+i) = ((Uint8*)buffer)[i];*/

}

SubspacePacket SubspacePacket::subStr(Uint start, Uint end) const
{
	SubspacePacket ret;

	if(start > size())
		return ret;

	if(end > size() || !end)
		end = static_cast<Uint>(size());

	if(end <= start) 
		return ret;

	ret.resize(end-start);
	for(Uint i=0; i < end-start; ++i)
	{
		//ret.packet.at(i) = packet.at(i + start);
		ret[i] = data[i + start];
	}

	// Returns a sub-packet. An 'end' value of 0 indicates upto the end of the packet.

	
	/*SubspacePacket ret;

	if(start > m_sPacket.size())
		return ret;

	if(end > m_sPacket.size() || !end)
		end = static_cast<Uint>(m_sPacket.size());

	if(end <= start) 
		return ret;

	ret.resize(end-start);
	if(ret.m_sPacket.capacity() < ret.size())
	{
		printf("WTF?\n");
	}
	for(Uint i=0; i < end-start; ++i)
	{
		//ret.packet.at(i) = packet.at(i + start);
		ret[i] = m_sPacket.at(i + start);
	}*/

	/*SubspacePacket ret;
	ret.SSPacket::operator =( SSPacket::subStr(start, end) );*/

	return ret;

}

Uint SubspacePacket::getSize() const
{
	return size();
}

unsigned long SubspacePacket::letol(char* c)
{
	return (c[0] | c[1] << 8 | c[2] << 16 | c[3] << 24);
}

unsigned short SubspacePacket::letos(char* c)
{
	return (c[0] | c[1] << 8);
}

void SubspacePacket::ltole(unsigned long l, char* c)
{
	/*c[0] = l & 0x000000FF;
	c[1] = l & 0x0000FF00;
	c[2] = l & 0x00FF0000;
	c[3] = l & 0xFF000000;*/

	c[0] = (char)(l >> 0 & 0xFF);
	c[1] = (char)(l >> 8 & 0xFF);
	c[2] = (char)(l >> 16 & 0xFF);
	c[3] = (char)(l >> 24 & 0xFF);
}

void SubspacePacket::stole(unsigned short s, char* c)
{
	/*c[0] = s & 0x00FF;
	c[1] = s & 0xFF00;*/

	c[0] = (char)(s >> 0 & 0xFF);
	c[1] = (char)(s >> 8 & 0xFF);
}