#include "SubspaceLVZManager.h"

#include "SubspaceHud.h"
#include "SubspaceLVZ.h"

SubspaceLVZManager::SubspaceLVZManager() :
	hud_(0)
{
}


SubspaceLVZManager::~SubspaceLVZManager()
{
	destroy();
}

bool SubspaceLVZManager::load(const string& path, const string& filename)
{
	SubspaceLVZ lvz;

	/*size_t b1 = filename_.find_last_of("/");
	size_t b2 = filename_.find_last_of("\\");

	if(b1 == filename_.npos) b1 = filename_.length();
	if(b2 == filename_.npos) b2 = filename_.length();
	size_t nameStart = min(b1, b2);

	if(nameStart == filename_.npos)
		nameStart = 0;

	string path = filename.substr(0, nameStart+1);*/
		
	if(!lvz.load(filename, path, true))
		return false;

	const vector<LVZImageObject>& images = lvz.getImageObjects();
	const vector<string>& imageFiles = lvz.getImageFiles();

	assert(images.size() == imageFiles.size());

	int imageOffset = (int)imageObjects_.size();			// accounts for loading multiple files

	for(size_t i=0; i < images.size(); ++i)
		addImageObject(path + imageFiles[i], images[i]);

	const vector<LVZMapObject>& mapObjs = lvz.getMapObjects();
	for(vector<LVZMapObject>::const_iterator m = mapObjs.begin(); m != mapObjs.end(); ++m)
	{
		LVZMapObject m2 = *m;
		m2.imageID += imageOffset;
		addMapObject(m2);
	}

	const vector<LVZScreenObject>& screenObjs = lvz.getScreenObjects();
	for(vector<LVZScreenObject>::const_iterator s = screenObjs.begin(); s != screenObjs.end(); ++s)
	{
		LVZScreenObject s2 = *s;
		s2.imageID += imageOffset;
		//if(s2.yType == LVZ_SCREEN_Normal)				// normal means xType takes priority
		//	s2.yType = s2.xType;
		addScreenObject(s2);
	}

	activateDisplayMode(LVZ_DISPLAYMODE_ShowAlways);

	return true;
}

void SubspaceLVZManager::setHud(SubspaceHud* hud)
{
	hud_ = hud;
}

void SubspaceLVZManager::activateDisplayMode(LVZDisplayModeType mode)
{
	ItemDataMap::iterator i;
	for(i = itemData_.begin(); i != itemData_.end(); ++i)
	{
		double displayTime = 0;
		int displayMode = -1;
		
		LVZMapObject* m = findMapObject(i->first);
		if(m)
		{
			displayMode = m->displayMode;
			displayTime = m->displayTime;						
		}
		else
		{
			LVZScreenObject* s = findScreenObject(i->first);
			if(s && s->displayMode == mode)
			{
				displayMode = s->displayMode;
				displayTime = s->displayTime;
			}
		}

		if(displayMode == mode)
		{
			(i->second).isEnabled = true;
			(i->second).displayTimeLeft = displayTime * 100.0;	// 1/10th s to ms		
		}
	}
}

bool SubspaceLVZManager::isObjectOn(int id) const
{
	ItemDataMap::const_iterator i = itemData_.find(id);
	if(i == itemData_.end())
		return false;

	return (*i).second.isEnabled;	
}

void SubspaceLVZManager::setObject(int id, bool status)
{
	ItemDataMap::iterator f = itemData_.find(id);
	ItemDataMap::iterator i;
	for(i = f; i != itemData_.end(); ++i)
	{
		if((*i).first == id)
			(*i).second.isEnabled = status;
	}
}

void SubspaceLVZManager::toggleObject(int id)
{
	setObject(id, !isObjectOn(id));
	/*ItemDataMap::iterator f = itemData_.find(id);
	ItemDataMap::iterator i;
	for(i = f; i != itemData_.end(); ++i)
	{
		if((*i).first == id)
			(*i).second.isEnabled = !(*i).second.isEnabled;
	}*/	
}


void SubspaceLVZManager::drawAllMapObjects(LVZLayerType layer)
{
	MapObjectMap::iterator i;
	for(i = mapObjects_[layer].begin(); i != mapObjects_[layer].end(); ++i)
	{
		const LVZMapObject* mo = i->second;
		
		ImageObjectMap::iterator imgIter = imageObjects_.find(mo->imageID);
		assert(imgIter != imageObjects_.end());				

		TextureAnimated& img = imgIter->second;
		glPushMatrix();
			glTranslated(mo->xCoord, (double)mo->yCoord-(double)img.getFrameHeight(), 0.0);
			img.display();
		glPopMatrix();
	}
}


void SubspaceLVZManager::drawMapObjects(double x, double y, double xEnd, double yEnd, LVZLayerType layer)
{
	assert(layer >= 0 && layer < LVZ_NumLayers);

	MapObjectMap::iterator i;
	for(i = mapObjects_[layer].begin(); i != mapObjects_[layer].end(); ++i)
	{
		const LVZMapObject* mo = i->second;
		
		ImageObjectMap::iterator imgIter = imageObjects_.find(mo->imageID);
		assert(imgIter != imageObjects_.end());	

		if(!itemData_[i->first].isEnabled)
			continue;

		TextureAnimated& img = imgIter->second;
		if(x < mo->xCoord+img.getWidth() && mo->xCoord < xEnd &&
			y < mo->yCoord && mo->yCoord-img.getHeight() < yEnd)
		{
			glPushMatrix();
				glTranslated(mo->xCoord, (double)mo->yCoord-(double)img.getFrameHeight(), 0.0);		// adjust for top-left coord to lower-left coord
				img.display();
			glPopMatrix();
		}
	}
}

void SubspaceLVZManager::drawScreenObjects(LVZLayerType layer)
{
	double offsetX = 0, offsetY = 0;

	assert(layer >= 0 && layer < LVZ_NumLayers);

	ScreenObjectMap::iterator i;
	for(i = screenObjects_[layer].begin(); i != screenObjects_[layer].end(); ++i)
	{
		const LVZScreenObject* so = i->second;
		
		ImageObjectMap::iterator imgIter = imageObjects_.find(so->imageID);
		assert(imgIter != imageObjects_.end());			

		if(!itemData_[i->first].isEnabled)
			continue;
	
		if(hud_)
		{
			offsetX = hud_->getLVZScreenCoordX((LVZScreenType)so->xType);
			offsetY = hud_->getLVZScreenCoordY((LVZScreenType)so->yType);
		}		

		TextureAnimated& img = imgIter->second;
		glPushMatrix();
			glTranslated(offsetX+so->xCoord, offsetY-so->yCoord-img.getFrameHeight(), 0.0);
			img.display();
		glPopMatrix();		
	}
}


void SubspaceLVZManager::update(double timestep)
{
	ImageObjectMap::iterator i;
	for(i = imageObjects_.begin(); i != imageObjects_.end(); ++i)
	{
		TextureAnimated& img = i->second;
		img.advanceFrame(timestep);
	}

	ItemDataMap::iterator d;
	for(d = itemData_.begin(); d != itemData_.end(); ++d)
	{
		LVZItemData& data = d->second;
		if(data.isEnabled && data.displayTimeLeft > 0)		// 0 and enabled is on forever
		{
			data.displayTimeLeft -= timestep;
			if(data.displayTimeLeft < 0)
			{
				data.isEnabled = false;
				data.displayTimeLeft = 0;
			}
		}
	}
	// TODO: update display time too
}

void SubspaceLVZManager::addImageObject(const string& filename, const LVZImageObject& obj)
{
	TextureAnimated newTex;
	if(newTex.load(filename, false))
	{
		newTex.setFrames(obj.yCount, obj.xCount);	
	}
	int numFrames = obj.xCount * obj.yCount;
	newTex.setAnimationPeriod(obj.displayTime*10.0);		// convert 1/100th s to ms

	imageObjects_[(int)imageObjects_.size()] = newTex;
}

void SubspaceLVZManager::addMapObject(const LVZMapObject& o)
{
	LVZMapObject* obj = new LVZMapObject(o);
	obj->yCoord = 1024*16 - obj->yCoord;

	ImageObjectMap::iterator imgIter = imageObjects_.find(obj->imageID);
	if(imgIter == imageObjects_.end())
		return;

	TextureAnimated& img = imgIter->second;
	if(img.getNumFrames() == 0 || img.getNumFrames() == (Uint)-1)
		return;

	mapObjects_[obj->layer].insert(std::pair<int, LVZMapObject*>(obj->objectID, obj));

	if(itemData_.find(obj->objectID) == itemData_.end())
	{
		LVZItemData item;
		item.isEnabled = false;
		item.displayTimeLeft = 0;//obj->displayTime*100.0;		// convert 1/10th s to ms

		itemData_.insert(std::pair<int, LVZItemData>(obj->objectID, item));	
	}
}

void SubspaceLVZManager::addScreenObject(const LVZScreenObject& o)
{
	if(!objHasImage(o))
		return;

	LVZScreenObject* obj = new LVZScreenObject(o);

	screenObjects_[obj->layer].insert(std::pair<int, LVZScreenObject*>(obj->objectID, obj));
	
	if(itemData_.find(obj->objectID) == itemData_.end())
	{
		LVZItemData item;
		item.isEnabled = false;
		item.displayTimeLeft = 0;//obj->displayTime*100.0;		// convert 1/10th s to ms

		itemData_.insert(std::pair<int, LVZItemData>(obj->objectID, item));
	}
}

void SubspaceLVZManager::destroy()
{
	for(int l=0; l < LVZ_NumLayers; ++l)
	{
		MapObjectMap::iterator m;
		for(m = mapObjects_[l].begin(); m != mapObjects_[l].end(); ++m)
		{
			assert(m->second);
			delete m->second;
		}

		ScreenObjectMap::iterator s;
		for(s = screenObjects_[l].begin(); s != screenObjects_[l].end(); ++s)
		{
			assert(s->second);
			delete s->second;
		}
	}
}

template <class T>
bool SubspaceLVZManager::objHasImage(const T& obj)
{
	return (imageObjects_.find(obj.imageID) != imageObjects_.end());	
}

LVZMapObject* SubspaceLVZManager::findMapObject(int id)
{
	for(int l=0; l < LVZ_NumLayers; ++l)
	{
		MapObjectMap::iterator m = mapObjects_[l].find(id);
		if(m != mapObjects_[l].end())
			return m->second;
	}
	return 0;
}

LVZScreenObject* SubspaceLVZManager::findScreenObject(int id)
{
	for(int l=0; l < LVZ_NumLayers; ++l)
	{
		ScreenObjectMap::iterator s = screenObjects_[l].find(id);
		if(s != screenObjects_[l].end())
			return s->second;
	}
	return 0;
}