#ifndef _FILELOGGER_H_
#define _FILELOGGER_H_

#include <string>

#include <windows.h>
#include <stdarg.h>
#include <stdio.h>

using std::string;

//DONT USE THIS: use global StreamDecorators instead

class FileLogger
{
public:

	FileLogger();
	FileLogger(const string& header, bool showTimestamp);
	virtual ~FileLogger();

	virtual const FILE* getLog() const;
	virtual void setLog(FILE* file);

	virtual void setHeader(const string& header);
	virtual string getHeader() const;

	virtual void setShowTimestamp(bool show);
	virtual bool getShowTimestamp() const;
	
	virtual void log(const char* format, ...) const;
	virtual void debugLog(const char* format, ...) const;

private:

	FILE* logFile;
	FILE* debugLogFile;

	string header;
	bool showTimestamp;
};

#endif