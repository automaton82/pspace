//David Millman

#include <string>
#include <vector>
#include <math.h>
#include <windows.h>

#ifndef _Bitmap_
#define _Bitmap_

/**
* Defines a bitmap object. 
*
* @author	Dave Millman (millmd@rpi.edu)
*/
class Bitmap
{
public:
	Bitmap();

	BITMAPFILEHEADER* getFileHeader()	{ return &fileHeader_; }
	BITMAPINFOHEADER* getInfoHeader()	{ return &infoHeader_; }
	RGBQUAD* getColorIndices()			{ return colorIndices_; }
	unsigned char* getRawData()			{ return rawData_; }
	unsigned char* getColorData()		{ return colorData_; }
	unsigned char* getTileData(int n, int tileWidth, int tileHeight);

	int getHeight()						{ return infoHeader_.biHeight; }
	int getWidth()						{ return infoHeader_.biWidth; }

	///loads a file into a bitamp
	bool loadFromFile(std::string filename);

	bool load(FILE* file);

	///loads a bitmap from an open handle (file)
	bool loadFromHandle(HANDLE h);

private:
	static bool isBitmapType(int t);
	static bool isCompressed(int c);
	bool convertRawData();
	bool uncompressRawData();

	bool readColorIndexes(HANDLE h);

	BITMAPFILEHEADER fileHeader_;
	BITMAPINFOHEADER infoHeader_;

	RGBQUAD* colorIndices_;
	unsigned char* rawData_;
	unsigned char* colorData_;

	int currentByte_;
};

#endif