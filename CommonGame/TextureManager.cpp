//David Millman

#include "TextureManager.h"

#include "AsciiUtil.h"

#include <vector>
using std::vector;

#include "BmpObject.h"
#include "PngObject.h"

TextureManager::TextureManager()
{
	TextureData* blank = new TextureData();
	this->addItem(blank);		//make texture ids start at 1
}

int TextureManager::loadTexture(const string& filename, bool stretch)
{														//TODO: very convoluded function
	TextureData* newData = 0;
	int id = -1;
	
	if(files[filename] == 0)	//new file
	{
		newData = new TextureData();
			
		if(loadTextureType(filename, newData, stretch))	//load the texture
		{
			id = this->addItem(newData);
			if(id >= 0)
			{
				printf("Texture data loaded [%d]: %s\n", id, filename.c_str());
				files[filename] = id;	//store filename to help caching
			}
		}
	}
	else	//texture already exists
	{
		id = files[filename];
	}

	return id;
}

bool TextureManager::loadTextureType(const string& filename, TextureData* data, bool stretch)
{
	assert(data);
	bool retval = false;

	vector<string> tokens = AsciiUtil::tokenize(filename, ".");

	if(tokens.size() == 0)
		return false;

	bool useStretch_ = stretch;

	string ext = AsciiUtil::tolower(tokens.back());
	if(ext == "bmp" || ext == "bm2")
	{
		BmpObject bmp;
		retval = bmp.load(filename);
		if(retval)
		{
			//data->setColorKey(Color(0, 0, 0));
			data->loadFromMemory(bmp.getData(), bmp.getWidth(), bmp.getHeight(), useStretch_);
		}
	}
	else if(ext == "png")
	{
		PngObject png;
		retval = png.load(filename);
		if(retval)
			data->loadFromMemory(png.getData(), png.getWidth(), png.getHeight(), useStretch_);
	}

	return retval;
}

bool TextureManager::unloadTexture(const string& filename)
{
	int id = files[filename];
	bool result = false;

	if(id > 0)
	{
		result = this->deleteItem(id);
	}

	return result;
}