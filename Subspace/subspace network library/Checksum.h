//This file was Kindly provided by Snrrrub to handle all the Checksums

#include <iostream>
#include "DataTypes.h"

// <-<-<- Snrrrub ->->->
#ifndef Checksum_H_
#define Checksum_H_

class Checksum
{
	public:
		Checksum()
		{
			exeData = NULL;
		}
		~Checksum()
		{
			if(exeData)
				delete[] exeData;
		}
		bool LoadMap(const char *filename, void *dest);
		Uint32 exeChecksum(Uint32 key, const char *filename);
		Uint32 mapChecksum(Uint32 key, void *data);
		Uint32 settingsChecksum(Uint32 key, void *settings);
		Uint32 fileChecksum(void *fileData, Uint32 len);
		Uint32 fileChecksum(const char *filename);

		Uint32 generateEXEChecksum(Uint32 key);

	private:
		Uint32 partialExeSum(Uint8 *exeData, Uint32 key, Uint32 cnt, Uint32 index);
		inline void generate4(Uint32 *offset, Uint32 key);
		inline void generate16(Uint32 *offset, Uint32 key);
		inline void generate64(Uint32 *offset, Uint32 key);
		void generateDict(Uint32 *offset, Uint32 key);

		Uint8 *exeData;
		Uint32 dict[256];
};

#endif