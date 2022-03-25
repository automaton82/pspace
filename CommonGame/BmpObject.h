#ifndef _BMPOBJECT_H_
#define _BMPOBJECT_H_

#include <string>
using std::string;

#include "stdio.h"

/*
Intended for loading BMP image files for use with opengl
*/

class BmpObject
{
public:
	BmpObject();
	BmpObject(const string& filename);
	~BmpObject();

	char* getData() const;
	unsigned int getBitDepth() const;
	unsigned int getHeight() const;
	unsigned int getImageSize() const;
	unsigned int getWidth() const;

	bool load(const string& filename);	//assumes 24-bit bmp, converts to 32-bit
	//bool save();

	void scaleAlpha();		//scales alpha channel based on color

private:

	void addAlphaChannel();
	void clearData();
	bool readBmpFile(const string& filename);

private:

	unsigned int width_, height_;
	char* data_;

	unsigned int bitDepth_;
};

#endif 