#ifndef _SUBSPACECHATSTREAM_H_
#define _SUBSPACECHATSTREAM_H_

#include <string>
using std::string;

#include "AsciiUtil.h"
#include "DataTypes.h"
#include "StreamDecorator.h"
class SubspaceChat;

//manipulator prototypes
class setColor;

//SubspaceChatStream class
class SubspaceChatStream : 
	public OutputStreamDecorator
{
public:
	SubspaceChatStream();
	SubspaceChatStream(SubspaceChat* buffer);
	
	void setChatBuffer(SubspaceChat* buffer);
	void setColor(Uchar color);

	//Stream functions
	void flush();
	void put(char c);
	void write(const char* s, int size);
	void write(const string& s);
			
			//TODO: find a way to use overrides for the streaming
	//Stream operators
	SubspaceChatStream& operator<< (bool val)			{ write(AsciiUtil::itoa(val)); return *this; }
	SubspaceChatStream& operator<< (short val)			{ write(AsciiUtil::itoa(val)); return *this; }
	SubspaceChatStream& operator<< (unsigned short val)	{ write(AsciiUtil::itoa(val)); return *this; }
	SubspaceChatStream& operator<< (int val)			{ write(AsciiUtil::itoa(val)); return *this; }
	SubspaceChatStream& operator<< (unsigned int val)	{ write(AsciiUtil::itoa(val)); return *this; }
	SubspaceChatStream& operator<< (long val)			{ write(AsciiUtil::itoa(val)); return *this; }
	SubspaceChatStream& operator<< (unsigned long val)	{ write(AsciiUtil::itoa(val)); return *this; }
	SubspaceChatStream& operator<< (float val)			{ write(AsciiUtil::ftoa(val, 2)); return *this; }
	SubspaceChatStream& operator<< (double val)			{ write(AsciiUtil::ftoa(val, 2)); return *this; }
	SubspaceChatStream& operator<< (long double val)	{ write(AsciiUtil::ftoa(val, 2)); return *this; }
	//SubspaceChatStream& operator<< (void* val)			{ write(AsciiUtil::itoa((int)val)); return *this; }
	
	friend SubspaceChatStream& operator<< (SubspaceChatStream& s, char val)			{ s.put(val); return s; }
	friend SubspaceChatStream& operator<< (SubspaceChatStream& s, signed char val)		{ s.put(val); return s; }
	friend SubspaceChatStream& operator<< (SubspaceChatStream& s, unsigned char val)	{ s.put(val); return s; }

	friend SubspaceChatStream& operator<< (SubspaceChatStream& s, const char* val)		{ s.write(val); return s; }
	friend SubspaceChatStream& operator<< (SubspaceChatStream& s, const string& val)	{ s.write(val); return s; }
	
	SubspaceChatStream& operator<< (SubspaceChatStream& (*pf)(SubspaceChatStream&)) { (*pf)(*this); return (*this); }
	
	//Manipulators
	friend SubspaceChatStream& endl(SubspaceChatStream& s)	{ s.put('\n'); s.flush(); return s; }
	friend SubspaceChatStream& ends(SubspaceChatStream& s)	{ s.flush(); return s; }

public:
	
	Uchar currentColor_;
	SubspaceChat* buffer_;
	string currentBuffer_;
};

//TODO: turn this into a template stream manipulator class

//SubspaceChatStream manipulators
class setColor
{
public:
	setColor(Uchar c) : color_(c)
	{}
	
	template <class T>
	friend T& operator<<(T& stream, const setColor& sc) { stream.setColor(sc.color_); return stream; }

private:

	Uchar color_;
};


#endif