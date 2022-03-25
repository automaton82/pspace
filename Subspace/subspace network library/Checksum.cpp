//This file was Kindly provided by Snrrrub to handle all the Checksums

// <-<-<- Writen By: Snrrrub ->->->

#include "Checksum.h"

bool Checksum::LoadMap(const char *filename, void *dest)
{
	FILE *fp = fopen(filename, "rb");
	Uint32 skipSize, fileSize;
	char *ptr = (char *)dest;

	if(!fp)
		return false;

	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if(fgetc(fp) == 'B' && fgetc(fp) == 'M')
	{
		fread(&skipSize, 4, 1, fp);
		fseek(fp, skipSize, SEEK_SET);
		fileSize -= skipSize;
	}
	else
		fseek(fp, 0, SEEK_SET);
	for(Uint32 cnt = 0; cnt < fileSize; cnt += 4)
	{
		Uint32 data;
		fread(&data, 4, 1, fp);
		ptr[((data >> 0x0C) & 0xFFF) * 0x400 + (data & 0xFFF)] = (data >> 0x18) & 0xFF;
	}
	fclose(fp);
	return true;
}

Uint32 Checksum::mapChecksum(Uint32 key, void *data)
{
	Uint32 EAX, ECX, ESI, EDX;
	Uint32 HighBit = 0;
	Uint32 cnt;
	Uint32 original_key = key;

	if(!data)
		return 0;

	char *mapdata = (char *)data;
	
	EAX = key;

	if(EAX & 0x80000000)
		HighBit = 0xFFFFFFFF;

	EAX ^= HighBit;
	EAX -= HighBit;
	EAX &= 0x1F;
	EAX ^= HighBit;
	ECX = EAX - HighBit;
	if(ECX >= 0x400)
		return key;

	EDX = key % 0x1F;

	ESI = (ECX << 0x0A) + (Uint32)mapdata + EDX;
	EAX = 0x400 - EDX;
	cnt = (0x41F - ECX) >> 5;
	mapdata  = (char *)EAX;

	for(; cnt > 0; cnt--, ESI += 0x8000)
	{
		EDX = EAX + ESI;
		ECX = ESI;
		if(!(ESI < EDX))
			continue;
		while(ECX < EDX)
		{
			unsigned char byte = (*((char *)ECX));
			if((byte < 0xA1 || byte == 0xAB) && byte != 0)
				key += original_key ^ byte;
			ECX += 0x1F;
		}
		EAX = (Uint32)mapdata;
	}
	return key;
}

Uint32 Checksum::settingsChecksum(Uint32 key, void *settings)
{
	int i;
	Uint32 checksum = 0;
	Uint32 *ptr = (Uint32 *)settings;

	if(!settings)
		return 0;

	for(i = 0; i < 0x165; i++)
		checksum += ptr[i] ^ key;
	return checksum;
}

Uint32 Checksum::fileChecksum(void *fileData, Uint32 len)
{
	Uint32 index = 0;
	Uint32 key = 0xFFFFFFFF;
	Uint8 *buffer = (Uint8 *)fileData;
	generateDict(dict, 0);

	if(!fileData)
		return 0;

	for(Uint32 i = 0; i < len; i++)
	{
		index = dict[(key & 255) ^ buffer[i]];
		key = (key >> 8) ^ index;
	}
	
	return ~key;
}

Uint32 Checksum::exeChecksum(Uint32 key, const char *filename)
{
	Uint32 checksum = 0;
	if(!exeData)
	{
		Uint32 exeSize;
		FILE *fp = fopen(filename, "rb");
		if(!fp)
			return 0;
		fseek(fp, 0, SEEK_END);
		exeSize = ftell(fp);
		fseek(fp, 1024, SEEK_SET);

		if(exeSize <= 1024)
		{
			fclose(fp);
			return 0;
		}
		exeData = new Uint8 [exeSize - 1024];
		fread(exeData, exeSize - 1024, 1, fp);
		fclose(fp);
	}
	return	partialExeSum(exeData, key, 0xD5A0, 0x6540) + partialExeSum(exeData, key, 0x2480, 0x00) +
			partialExeSum(exeData, key, 0x6F70, 0x1C6E0) + partialExeSum(exeData, key, 0x21E0, 0x42A70) +
			partialExeSum(exeData, key, 0xD3C0, 0x301A0) + partialExeSum(exeData, key, 0x5FE0, 0x2A1B0) +
			partialExeSum(exeData, key, 0x1C90, 0x3560) + partialExeSum(exeData, key, 0x8BE0, 0x13AF0) + 
			partialExeSum(exeData, key, 0xE0, 0x5350);
}

Uint32 Checksum::partialExeSum(Uint8 *exeData, Uint32 key, Uint32 cnt, Uint32 index)
{
	Uint32 temp, PartialKey = 0;
		 
	while(cnt > 4)
	{
		memcpy(&temp, exeData + index, 4);
		if((cnt & 0x7FF) == 0)
		{
			temp |= key;
			PartialKey += temp;
		}
		else
		{
			temp ^= PartialKey;
			temp ^= key;
			PartialKey = temp;
		}
		cnt -= 3;
		index += 3;
	}
	return PartialKey;
}

Uint32 Checksum::fileChecksum(const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	Uint8 *data = NULL;
	Uint32 len = 0, ret = 0;

	if(!fp)
		return 0;

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	data = new Uint8[len];
	fread(data, len, 1, fp);
	fclose(fp);
	ret = fileChecksum(data, len);
	delete[] data;
	return ret;
}

// Thanks to Catid for cleaning this stuff up for me :) - Snrrrub

inline void Checksum::generate4(Uint32 *offset, Uint32 key)
{
	offset[0] = key;
	offset[1] = key ^ 0x77073096;
	offset[2] = key ^= (0x77073096 << 1);
	offset[3] = key ^ 0x77073096;
}

inline void Checksum::generate16(Uint32 *offset, Uint32 key)
{
	generate4(offset, key);
	generate4(offset + 4, key  ^ 0x076dc419);
	generate4(offset + 8, key  ^= (0x076dc419 << 1));
	generate4(offset + 12, key ^ 0x076dc419);
}

inline void Checksum::generate64(Uint32 *offset, Uint32 key)
{
	generate16(offset, key);
	generate16(offset + 16, key ^ 0x1db71064);
	generate16(offset + 32, key ^= (0x1db71064 << 1));
	generate16(offset + 48, key ^ 0x1db71064);
}

inline void Checksum::generateDict(Uint32 *offset, Uint32 key)
{
	generate64(offset, key);
	generate64(offset + 64, key ^ 0x76dc4190);
	generate64(offset + 128, key ^= (0x76dc4190 << 1));
	generate64(offset + 192, key ^ 0x76dc4190);
}

Uint32 Checksum::generateEXEChecksum(Uint32 key)
{
	Uint32 part, csum = 0;

	part = 0xc98ed41f;
	part += 0x3e1bc | key;
	part ^= 0x42435942 ^ key;
	part += 0x1d895300 | key;
	part ^= 0x6b5c4032 ^ key;
	part += 0x467e44 | key;
	part ^= 0x516c7eda ^ key;
	part += 0x8b0c708b | key;
	part ^= 0x6b3e3429 ^ key;
	part += 0x560674c9 | key;
	part ^= 0xf4e6b721 ^ key;
	part += 0xe90cc483 | key;
	part ^= 0x80ece15a ^ key;
	part += 0x728bce33 | key;
	part ^= 0x1fc5d1e6 ^ key;
	part += 0x8b0c518b | key;
	part ^= 0x24f1a96e ^ key;
	part += 0x30ae0c1 | key;
	part ^= 0x8858741b ^ key;
	csum += part;

	part = 0x9c15857d;
	part += 0x424448b | key;
	part ^= 0xcd0455ee ^ key;
	part += 0x727 | key;
	part ^= 0x8d7f29cd ^ key;
	csum += part;

	part = 0x824b9278;
	part += 0x6590 | key;
	part ^= 0x8e16169a ^ key;
	part += 0x8b524914 | key;
	part ^= 0x82dce03a ^ key;
	part += 0xfa83d733 | key;
	part ^= 0xb0955349 ^ key;
	part += 0xe8000003 | key;
	part ^= 0x7cfe3604 ^ key;
	csum += part;

	part = 0xe3f8d2af;
	part += 0x2de85024 | key;
	part ^= 0xbed0296b ^ key;
	part += 0x587501f8 | key;
	part ^= 0xada70f65 ^ key;
	csum += part;

	part = 0xcb54d8a0;
	part += 0xf000001 | key;
	part ^= 0x330f19ff ^ key;
	part += 0x909090c3 | key;
	part ^= 0xd20f9f9f ^ key;
	part += 0x53004add | key;
	part ^= 0x5d81256b ^ key;
	part += 0x8b004b65 | key;
	part ^= 0xa5312749 ^ key;
	part += 0xb8004b67 | key;
	part ^= 0x8adf8fb1 ^ key;
	part += 0x8901e283 | key;
	part ^= 0x8ec94507 ^ key;
	part += 0x89d23300 | key;
	part ^= 0x1ff8e1dc ^ key;
	part += 0x108a004a | key;
	part ^= 0xc73d6304 ^ key;
	part += 0x43d2d3 | key;
	part ^= 0x6f78e4ff ^ key;
	csum += part;

	part = 0x45c23f9;
	part += 0x47d86097 | key;
	part ^= 0x7cb588bd ^ key;
	part += 0x9286 | key;
	part ^= 0x21d700f8 ^ key;
	part += 0xdf8e0fd9 | key;
	part ^= 0x42796c9e ^ key;
	part += 0x8b000003 | key;
	part ^= 0x3ad32a21 ^ key;
	csum += part;

	part = 0xb229a3d0;
	part += 0x47d708 | key;
	part ^= 0x10b0a91 ^ key;
	csum += part;

	part = 0x466e55a7;
	part += 0xc7880d8b | key;
	part ^= 0x44ce7067 ^ key;
	part += 0xe4 | key;
	part ^= 0x923a6d44 ^ key;
	part += 0x640047d6 | key;
	part ^= 0xa62d606c ^ key;
	part += 0x2bd1f7ae | key;
	part ^= 0x2f5621fb ^ key;
	part += 0x8b0f74ff | key;
	part ^= 0x2928b332;
	csum += part;

	part = 0x62cf369a;
	csum += part;

	return csum;
}
