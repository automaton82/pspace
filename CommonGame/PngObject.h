#ifndef _PNGOBJECT_H_
#define _PNGOBJECT_H_

#include <string>
using std::string;

#include <stdio.h>

/*
Intended for loading PNG image files for use with opengl
*/

class PngObject
{
public:
	PngObject();
	PngObject(const string& filename);
	~PngObject();

	bool load(const string& filename);
	//bool save();

	char* getData() const;
	unsigned int getBitDepth() const;
	unsigned int getHeight() const;
	unsigned int getImageSize() const;
	unsigned int getWidth() const;

private:

	static const unsigned int pngBytesToCheck = 4;

private:

	void clearData();
	bool isPngFile(FILE* file);
	bool readPngFile(FILE* file);

	unsigned int width_, height_;
	char* data_;

	unsigned int bitDepth_;
};

#endif 