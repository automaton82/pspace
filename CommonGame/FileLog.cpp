#include "FileLog.h"

FileLog::FileLog() :
	setFile(stdout)
{
}

FileLog::~FileLog()
{
}

void FileLog::write(const char* format, ...)
{
	string s;
	
	s.resize(size);
	va_list argList;

	vsprintf(buffer, format, argList);
	va_start(argList, format);
	vprintf(writeArgs(format, argList);
	va_end(argList);
}

void FileLog::writeString(const string& s)
{
	fprintf(file_, s.c_str());
}