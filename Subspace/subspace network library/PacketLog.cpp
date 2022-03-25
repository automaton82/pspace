#include "PacketLog.h"

#include <iostream>
#include <stdarg.h>

int PacketLog::tabs_ = 0;
FILE* PacketLog::outFile_ = 0;

PacketLog::PacketLog()
{
	outFile_ = fopen("packet.txt", "w");
}

PacketLog::~PacketLog()
{
	if(outFile_)
		fclose(outFile_);
}

void PacketLog::log(const char* format, ...)
{
	if(!outFile_)
		return;

	for(int i=0; i < tabs_; ++i)
		fprintf(outFile_, "\t");

	va_list argList;

	va_start(argList, format);
	vfprintf(outFile_, format, argList);
	va_end(argList);
}

void PacketLog::tab()
{
	++tabs_;
}

void PacketLog::untab()
{
	if(tabs_ > 0)
		--tabs_;
}