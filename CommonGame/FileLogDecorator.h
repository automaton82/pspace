#ifndef _FILELOGDECORATOR_H_
#define _FILELOGDECORATOR_H_

#include "FileLogBase.h"

class FileLogDecorator : 
	public FileLogBase
{
public:
	virtual ~FileLogDecorator();
	FileLogDecorator(FileLogBase* base);

protected:

	virtual void writeArgs(const char* format, va_list argList) const;

private:

	FileLogBase* base_;
};

#endif