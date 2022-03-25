#ifndef _FILELOG_H_
#define _FILELOG_H_

#include "FileLogBase.h"

class FileLog : 
	public FileLogBase
{
public:

	FileLog();
	virtual ~FileLogger();

	virtual void write(const char* format, ...) const;

protected:
	
	virtual void writeString(const string& s) const;
};

#endif