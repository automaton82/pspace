//David Millman

#include "TextureData.h"

#include "GLObject.h"

#include "math.h"

Color TextureData::defaultColorKey_(0, 0, 0);
bool TextureData::useDefaultColorKey_(false);

TextureData::TextureData() : 
	width(0), height(0), 
	glTextureId(-1),
	colorKey_(0,0,0),
	useColorKey_(false)
{
}

TextureData::~TextureData()
{
	/*if(data)
		delete [] data;*/
}

const Color& TextureData::getDefaultColorKey()
{
	return defaultColorKey_;
}

void TextureData::setColorKey(const Color& color)
{
	colorKey_ = color;
	useColorKey_ = true;
}

void TextureData::setDefaultColorKey(const Color& color)
{
	defaultColorKey_ = color;
	useDefaultColorKey_ = true;
}

void TextureData::setUseColorKey(bool use)
{
	useColorKey_ = use;
}

void TextureData::setUseDefaultColorKey(bool use)
{
	useDefaultColorKey_ = use;
}

/*bool TextureData::loadFromBitmap(const string& filename)
{	
	AUX_RGBImageRec* TextureImage[1];               // Create Storage Space For The Textures
	memset(TextureImage, 0, sizeof(AUX_RGBImageRec*) * 1);      // Set The Pointer To NULL
	
	if(!(TextureImage[0] = auxDIBImageLoad(filename.c_str())))
	{
		return false;
	}

	return loadFromMemory(reinterpret_cast<char*>(TextureImage[0]->data), TextureImage[0]->sizeX, TextureImage[0]->sizeY);
}*/


bool TextureData::loadFromMemory(char* memData, int w, int h, bool stretch)
{	
	unload();	//precautionary


	GLuint texId;

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texId);

	glTextureId = static_cast<int>(texId);

	glBindTexture(GL_TEXTURE_2D, texId); // Bind the texture
	
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	memWidth = width = w;
	memHeight = height = h;

	int newPad = 0;//(4 - (w*4)%4)==4 ? 0 : (4 - (w*4)%4);
	int oldPad = 0;//(4 - (w*3)%4)==4 ? 0 : (4 - (w*3)%4);

	size_t size = (width+newPad)*height * 4;
	char* data = 0;

	//Transform raw data

	if(!stretch)
	{		//copies memData into data - adds blank space on edges
		data = expandData(memData, memWidth, memHeight, &memWidth, &memHeight);	//adjust memory sizes
	}
	else	//copy data
	{
		data = new char[size];
		memcpy(data, memData, size * sizeof(char));
	}

	if(useColorKey_)
		filterColor(data, memWidth, memHeight, colorKey_);
	else if(useDefaultColorKey_)
        filterColor(data, memWidth, memHeight, defaultColorKey_);
		
	// Texture generation
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, memWidth, memHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
	delete [] data;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

bool TextureData::unload()
{
	if(glTextureId > 0)
	{
		GLuint id = static_cast<unsigned int>(glTextureId);
		glDeleteTextures(1, &id);

		glTextureId = -1;
		return true;
	}
    	
	return false;
}

char* TextureData::expandData(char* data, Uint width, Uint height, Uint* newWidth, Uint* newHeight)
{
	int newPad = 0;//(4 - (w*4)%4)==4 ? 0 : (4 - (w*4)%4);
	int oldPad = 0;//(4 - (w*3)%4)==4 ? 0 : (4 - (w*3)%4);

	char* expandedData = 0;
	Uint expandedWidth, expandedHeight;

	expandedWidth = (int)pow(2, (int)ceil( log10((double)width) / log10(2.0) ));		//nearest > power of 2
	expandedHeight = (int)pow(2, (int)ceil( log10((double)height) / log10(2.0) ));	//nearest > power of 2
	expandedData = new char[(expandedWidth+newPad) * expandedHeight * 4];

	*newWidth = expandedWidth;
	*newHeight = expandedHeight;

	/*if(expandedWidth == width && expandedHeight == height)
	{
		return data;
	}*/

	Uint x,y;
	for(y=0; y < expandedHeight; ++y)
	{
		for(x=0; x < expandedWidth; ++x)
		{
			size_t currPixelNew = (y*expandedWidth + x)*4;
			size_t currPixelOld = (y*width + x)*4;

			if(x < width && y < height)
			{
				expandedData[currPixelNew] = data[currPixelOld];
				expandedData[currPixelNew+1] = data[currPixelOld+1];
				expandedData[currPixelNew+2] = data[currPixelOld+2];
				expandedData[currPixelNew+3] = data[currPixelOld+3];
			}
			else
			{
				Uint lastWidth = min(x, width-1);		//copy end colors
				Uint lastHeight = min(y, height-1);	
				Uint lastPixel = (lastHeight*width + lastWidth) * 4;

				expandedData[currPixelNew] = data[lastPixel];
				expandedData[currPixelNew+1] = data[lastPixel + 1];
				expandedData[currPixelNew+2] = data[lastPixel + 2];
				expandedData[currPixelNew+3] = data[lastPixel + 3];
			}
		}
	}

	return expandedData;
}

char* TextureData::filterColor(char* data, Uint width, Uint height, const Color& keyColor)
{
	int oldPad = 0;//(4 - (w*3)%4)==4 ? 0 : (4 - (w*3)%4);
	Uint i,j;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			unsigned long index = ((i * (width+oldPad) + j) * 4);
			
			if((unsigned char)data[index] == keyColor.r &&
				(unsigned char)data[index + 1] == keyColor.g &&
				(unsigned char)data[index + 2] == keyColor.b)
			{	
				data[index + 3] = 0;
			}
			else
			{
				//keep old color
				//data[index + 3] = (char)255;

				//scale alpha based on color
				//newData[newIndex + 3] = max(max(newData[newIndex+0], newData[newIndex+1]), newData[newIndex+2]);
			}
		}
	}

	return data;
}