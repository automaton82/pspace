#include "StreamDecorator.h"

OutputStreamDecorator::OutputStreamDecorator() : 
//	std::ostream(0),
	baseStream_(0),
	baseStreamDec_(0)
//	wroteLine_(false)
{
}

OutputStreamDecorator::OutputStreamDecorator(std::ostream* baseStream) : 
//	std::ostream(0),
	baseStream_(baseStream),
	baseStreamDec_(0)
//	wroteLine_(false)
{
}

OutputStreamDecorator::OutputStreamDecorator(OutputStreamDecorator* baseStream) : 
//	std::ostream(0),
	baseStream_(0),
	baseStreamDec_(baseStream)
//	wroteLine_(false)
{
}

OutputStreamDecorator::~OutputStreamDecorator()
{
	if(baseStreamDec_)
		delete baseStreamDec_;
}

/*std::ostream* OutputStreamDecorator::getBaseStream() const
{
	return baseStream_;
}*/	

void OutputStreamDecorator::setBaseStream(std::ostream* baseStream)
{
	baseStream_ = baseStream;
	baseStreamDec_ = 0;
}

void OutputStreamDecorator::setBaseStream(OutputStreamDecorator* baseStream)
{
	baseStream_ = 0;
	baseStreamDec_ = baseStream;
}

/*void OutputStreamDecorator::prependLine()
{

}

void OutputStreamDecorator::postpendLine()
{

}*/

void OutputStreamDecorator::flush()
{
	assertStream();

	//postpendLine();

	//wroteLine_ = false;
	flushStream();
}

void OutputStreamDecorator::put(char c)
{
	assertStream();

	//prepend();

	putStream(c);
}

void OutputStreamDecorator::write(const char* s, int size)
{
	assertStream();

	//prepend();

	//writeStream(s, size);
	string str;
	str.append(s, size);
	write(str);
}

void OutputStreamDecorator::write(const string& s)
{
	assertStream();

	//prepend();

	writeStream(s.c_str(), (int)s.size());
}

OutputStreamDecorator& OutputStreamDecorator::operator<< (bool val)			
{ 
	assertStream();

	//prepend();

	streamStream(val); 
	return *this; 
}

OutputStreamDecorator& OutputStreamDecorator::operator<< (short val)		
{ 
	assertStream();

	//prepend();

	streamStream(val); 
	return *this; 
}

OutputStreamDecorator& OutputStreamDecorator::operator<< (unsigned short val)	
{
	assertStream();

	//prepend();

	streamStream(val); 
	return *this; 
}

OutputStreamDecorator& OutputStreamDecorator::operator<< (int val)			
{ 
	assertStream();
 
	//prepend();

	streamStream(val); 
	return *this; 
}

OutputStreamDecorator& OutputStreamDecorator::operator<< (unsigned int val)	
{ 
	assertStream();

	//prepend();

    streamStream(val); 
	return *this; 
}

OutputStreamDecorator& OutputStreamDecorator::operator<< (long val)			
{ 
	assertStream();

	//prepend();

	streamStream(val); 
	return *this; 
}
	
OutputStreamDecorator& OutputStreamDecorator::operator<< (unsigned long val)	
{ 
	assertStream();

	//prepend();

	streamStream(val); 
	return *this;
}
	
OutputStreamDecorator& OutputStreamDecorator::operator<< (float val)			
{ 
	assertStream();

	//prepend();

	streamStream(val);
	return *this; 
}

OutputStreamDecorator& OutputStreamDecorator::operator<< (double val)			
{ 
	assertStream();

    //prepend();

	streamStream(val); 
	return *this; 
}
	
OutputStreamDecorator& OutputStreamDecorator::operator<< (long double val)
{ 
	assertStream();

	//prepend();

	streamStream(val); 
	return *this; 
}
	
OutputStreamDecorator& OutputStreamDecorator::operator<< (void* val)			
{ 
	assertStream();

	//prepend();

	streamStream(val); 
	return *this; 
}

	
OutputStreamDecorator& OutputStreamDecorator::operator<< (OutputStreamDecorator& (*pf)(OutputStreamDecorator&)) 
{ 
	//prepend();

	(*pf)(*this); 
	return (*this);
}

/*OutputStreamDecorator& OutputStreamDecorator::operator<< (char val)			
{ 
	put(val); 
	return (*this); 
}

OutputStreamDecorator& OutputStreamDecorator::operator<< (const char* val)			
{ 
	write(val); 
	return (*this); 
}

OutputStreamDecorator& OutputStreamDecorator::operator<< (const string& val)			
{ 
	write(val); 
	return (*this); 
}*/

//FRIENDS
	
OutputStreamDecorator& operator<< (OutputStreamDecorator& s, char val)			
{ 
	s.put(val); 
	return s; 
}
	
OutputStreamDecorator& operator<< (OutputStreamDecorator& s, signed char val)		
{
	s.put(val); 
	return s;
}

OutputStreamDecorator& operator<< (OutputStreamDecorator& s, unsigned char val)	
{
   	s.put(val); 
	return s;
}

OutputStreamDecorator& operator<< (OutputStreamDecorator& s, const char* val)
{ 
	s.write(val); 
	return s; 
}

OutputStreamDecorator& operator<< (OutputStreamDecorator& s, const signed char* val)
{ 
	s.write((char*)val); 
	return s; 
}

OutputStreamDecorator& operator<< (OutputStreamDecorator& s, const unsigned char* val)
{ 
	s.write((char*)val); 
	return s; 
}

OutputStreamDecorator& operator<< (OutputStreamDecorator& s, const string& val)	
{ 
	s.write(val); 
	return s;
}

OutputStreamDecorator& endl(OutputStreamDecorator& s)	
{ 
	s.put('\n'); 
	s.flush(); 
	return s; 
}

OutputStreamDecorator& ends(OutputStreamDecorator& s)	
{ 
	s.flush(); 
	return s; 
}

/*void OutputStreamDecorator::prepend()
{
	if(!wroteLine_)
	{
		//(*base_) << "[" << getLocalTime24() << "] ";
		prependLine();
		wroteLine_ = true;
	}
}*/
