#ifndef _TIMESTREAM_H_
#define _TIMESTREAM_H_

#include "StreamDecorator.h"

class TimeStream : 
	public OutputStreamDecorator
{
public:
	TimeStream();
	TimeStream(OutputStreamDecorator* base);
	virtual ~TimeStream();

	virtual void flush();
	virtual void put(char c);
	virtual void write(const string& s);
	//virtual void prependLine();

private:

	bool isNewline_;
};

#endif