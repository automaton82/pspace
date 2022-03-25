#ifndef _OUTPUTSTREAMDECORATOR_H_
#define _OUTPUTSTREAMDECORATOR_H_

#include <iostream>
#include <assert.h>

#include <string>
using std::string;

class OutputStreamDecorator
{
public:
	OutputStreamDecorator();
	OutputStreamDecorator(std::ostream* baseStream);
	OutputStreamDecorator(OutputStreamDecorator* baseStream);
	virtual ~OutputStreamDecorator();
	
	//Accessors
	//std::ostream* getBaseStream() const;
	void setBaseStream(std::ostream* baseStream);
	void setBaseStream(OutputStreamDecorator* baseStream);

	//Common decorator functionality
	//virtual void prependLine();		//Note: this assumes lines are always flushed - prepend happens every new flush
	//virtual void postpendLine();	//TODO: what's the real word for this

	//Stream functions
	virtual void flush();
	virtual void put(char c);
	virtual void write(const char* s, int size);	
	virtual void write(const string& s);

	//Stream operators
	virtual OutputStreamDecorator& operator<< (bool val);	
	virtual OutputStreamDecorator& operator<< (short val);
	virtual OutputStreamDecorator& operator<< (unsigned short val);
	virtual OutputStreamDecorator& operator<< (int val);
	virtual OutputStreamDecorator& operator<< (unsigned int val);
	virtual OutputStreamDecorator& operator<< (long val);
	virtual OutputStreamDecorator& operator<< (unsigned long val);
	virtual OutputStreamDecorator& operator<< (float val);
	virtual OutputStreamDecorator& operator<< (double val);
	virtual OutputStreamDecorator& operator<< (long double val);
	virtual OutputStreamDecorator& operator<< (void* val);
	virtual OutputStreamDecorator& operator<< (OutputStreamDecorator& (*pf)(OutputStreamDecorator&));

	/*OutputStreamDecorator& operator<< (char val);
	OutputStreamDecorator& operator<< (const char* val);
	OutputStreamDecorator& operator<< (const string& val);*/
	
	friend OutputStreamDecorator& operator<< (OutputStreamDecorator& s, char val);
	friend OutputStreamDecorator& operator<< (OutputStreamDecorator& s, signed char val);
	friend OutputStreamDecorator& operator<< (OutputStreamDecorator& s, unsigned char val);

	friend OutputStreamDecorator& operator<< (OutputStreamDecorator& s, const char* val);
	friend OutputStreamDecorator& operator<< (OutputStreamDecorator& s, const signed char* val);	
	friend OutputStreamDecorator& operator<< (OutputStreamDecorator& s, const unsigned char* val);
	friend OutputStreamDecorator& operator<< (OutputStreamDecorator& s, const string& val);

	//Manipulators
	friend OutputStreamDecorator& endl(OutputStreamDecorator& s);
	friend OutputStreamDecorator& ends(OutputStreamDecorator& s);

protected:

	void prepend();
	void postpend();

protected:
	std::ostream* baseStream_;
	OutputStreamDecorator* baseStreamDec_;

	//bool wroteLine_;
};

#define assertStream() (assert(baseStream_ || baseStreamDec_))

#define flushStream() \
	if(baseStream_) \
		baseStream_->flush();  \
	else if(baseStreamDec_)  \
		baseStreamDec_->flush();

#define streamStream(stream)	\
	if(baseStream_)				\
		(*baseStream_) << stream;\
	else if(baseStreamDec_)		\
		(*baseStreamDec_) << stream;

#define putStream(c) \
	if(baseStream_) \
		baseStream_->put(c);  \
	else if(baseStreamDec_)  \
		baseStreamDec_->put(c);

#define writeStream(string, size) \
	if(baseStream_) \
		baseStream_->write(string, size);  \
	else if(baseStreamDec_)  \
		baseStreamDec_->write(string, size);

#endif