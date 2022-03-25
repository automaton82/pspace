#ifndef _FILELOGBASE_H_
#define _FILELOGBASE_H_

#include <stdio.h>
#include <string>
using std::string;

// file logger decorators

class FileLogBase
{
public:

	virtual ~FileLogBase();

	virtual const FILE* getFile() const;
	virtual void setFile(FILE* file);

	virtual void write(const char* format, ...) const = 0;

protected:
	
	virtual void writeString(const string& s) const = 0;

private:

	FILE* file_;
};

#endif