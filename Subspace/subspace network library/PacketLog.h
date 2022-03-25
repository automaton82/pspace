#ifndef _PACKETLOG_H_
#define _PACKETLOG_H_

#include <fstream>

class PacketLog
{
public:
	PacketLog();
	~PacketLog();

	static void log(const char* format, ...);

	static void tab();
	static void untab();

private:

	static int tabs_;

	static FILE* outFile_;
};

#endif