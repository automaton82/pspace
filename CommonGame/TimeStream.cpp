#include "TimeStream.h"

#include "WinUtil.h"

TimeStream::TimeStream() : 
	OutputStreamDecorator(&std::cout),
	isNewline_(true)
{
	streamStream("[" << getLocalTime24() << "] ");
}

TimeStream::TimeStream(OutputStreamDecorator* base) :
	OutputStreamDecorator(base),
	isNewline_(true)
{
	streamStream("[" << getLocalTime24() << "] ");
}

TimeStream::~TimeStream()
{
}

void TimeStream::flush()
{
	OutputStreamDecorator::flush();

	//streamStream("[" << getLocalTime24() << "] ");

	isNewline_ = true;
}

void TimeStream::put(char c)
{
	if(isNewline_)
	{
		streamStream("[" << getLocalTime24() << "] ");
		isNewline_ = false;
	}

	OutputStreamDecorator::put(c);	
}

void TimeStream::write(const string& s)
{
	if(isNewline_)
	{
		streamStream("[" << getLocalTime24() << "] ");
		isNewline_ = false;
	}

	OutputStreamDecorator::write(s);
}

/*void TimeStream::prependLine()
{
	streamStream("[" << getLocalTime24() << "] ");
}*/