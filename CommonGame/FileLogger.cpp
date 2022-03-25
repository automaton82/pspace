#include "FileLogger.h"

FileLogger::FileLogger() : 
	logFile(stdout),
	header("FileLogger"),
	showTimestamp(true)
{
	debugLogFile = fopen("debugLog.txt", "w");
	//logFile = fopen("log.txt", "w");
}

FileLogger::FileLogger(const string& head, bool show) :
	logFile(stdout),
	header(header),
	showTimestamp(show)
{
	debugLogFile = fopen("debugLog.txt", "w");
	//logFile = fopen("log.txt", "w");
}

FileLogger::~FileLogger()
{
}

const FILE*	FileLogger::getLog() const
{
	return logFile;
}

void FileLogger::setLog(FILE* file)
{
	logFile = file;
}

void FileLogger::setHeader(const string& h)
{
	header = h;
}

string FileLogger::getHeader() const
{
	return header;
}

void FileLogger::setShowTimestamp(bool show)
{
	showTimestamp = show;
}

bool FileLogger::getShowTimestamp() const
{
	return showTimestamp;
}

void FileLogger::log(const char* format, ...) const
{
	char buf[32];
	string logStr;
	if(showTimestamp)
	{
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);

		sprintf(buf, "[%.02u:%.02u:%.02u] ", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		logStr += buf;
	}

	if(header.length() > 0)
	{
		logStr += header + "\t";
	}
	logStr += string(format) + "\n";

	va_list argList;

	va_start(argList, format);
	vfprintf(logFile, logStr.c_str(), argList);
	va_end(argList);
}

void FileLogger::debugLog(const char* format, ...) const
{
	char buf[32];
	string logStr;
	if(showTimestamp)
	{
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);

		sprintf(buf, "[%.02u:%.02u:%.02u] ", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		logStr += buf;
	}

	if(header.length() > 0)
	{
		logStr += header + "\t";
	}
	logStr += string(format) + "\n";

	va_list argList;

	va_start(argList, format);

#ifdef _DEBUG
    vfprintf(debugLogFile, logStr.c_str(), argList);
	//vfprintf(logFile, logStr.c_str(), argList);
#else
	vfprintf(debugLogFile, logStr.c_str(), argList);
#endif

	va_end(argList);
}
