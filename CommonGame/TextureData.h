//David Millman

#ifndef _TEXTUREDATA_H_
#define _TEXTUREDATA_H_

#include "DataTypes.h"
#include "Color.h"

#include <assert.h>
#include <string>

using std::string;

class TextureData
{
public:
	
	TextureData();
	~TextureData();

			//TODO: make sure when this is loading the render context is correct
	
	static const Color& getDefaultColorKey();

	void setColorKey(const Color& color);
	static void setDefaultColorKey(const Color& color);
	static void setUseDefaultColorKey(bool use);
	void setUseColorKey(bool use);
    
	//bool loadFromBitmap(const string& filename);
	//assumes 32-bit RGBA data
	bool loadFromMemory(char* memData, int width, int height, bool stretch = true);		//if stretch=false and the width/height are not a power of 2, the image is padded with <endcolor> to make it a power of 2
	bool unload();

private:

	static char* expandData(char* data, Uint width, Uint height, Uint* newWidth, Uint* newHeight);	//copies data into return value - old data still exists
	static char* filterColor(char* data, Uint width, Uint height, const Color& keyColor);	//assumes 32-bit data, makes any pixel with color have 0 alpha (fully transparent), matches color with only RGB, not RGBA

public:
	
	int glTextureId;
	Uint width, height;
	Uint memWidth, memHeight;	//width including padding

private:
	
	bool useColorKey_;

	Color colorKey_;
	
	static Color defaultColorKey_;
	static bool useDefaultColorKey_;

};

#endif