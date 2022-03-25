#ifndef _DISPLAYOBJECTMANAGER_H_
#define _DISPLAYOBJECTMANAGER_H_

#include "Database.h"
#include "Singleton.h"

class DisplayObjectManager
{
public:

	int loadObjectFromTexture(const string& name, const string& textureFile);
	int loadObjectFromModel(const string& name, const string& modelFile);

	void drawObject(int displayObjectId);
	
private:
	
	Database<DisplayObject> displayObjects_;
};

static DisplayObjectManager& displayObjectManager = *Singleton<DisplayObjectManager>::instance();

#endif