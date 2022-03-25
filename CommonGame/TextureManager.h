
//David Millman

#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include <string>
#include <map>

#include "Singleton.h"
#include "Database.h"
#include "TextureData.h"

using std::string;
using std::map;

//TODO: reorganize this - manager should use utility loading functions - manager should just handle texture datas

class TextureManager :
	public Database<TextureData>
{
public:

	typedef map<string, int> TextureMap;

public:

	TextureManager();

	int loadTexture(const string& filename, bool stretch);
	bool unloadTexture(const string& filename);

private:
	//make this actually load specified type
	bool loadTextureType(const string& filename, TextureData* data, bool stretch);
	
	TextureMap files;
};

//TODO: phase this out
static TextureManager& textureManager = *Singleton<TextureManager>::instance();

#endif