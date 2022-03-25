#ifndef _SUBSPACELVZMANAGER_H_
#define _SUBSPACELVZMANAGER_H_

#include <hash_map>
#include <map>
#include <string>

using std::hash_map;
using std::hash_multimap;
using std::map;
using std::multimap;
using std::string;

#include "TextureAnimated.h"

class SubspaceHud;
#include "SubspaceLVZData.h"

class SubspaceLVZManager
{
public:

	SubspaceLVZManager();
	~SubspaceLVZManager();

	bool load(const string& path, const string& filename);			// loads LVZ file
	void setHud(SubspaceHud* hud);				// for screen positions

	void activateDisplayMode(LVZDisplayModeType mode);
	bool isObjectOn(int id) const;
	void setObject(int id, bool isEnabled);	
	void toggleObject(int id);	

	///////////////////////////////////

	void drawAllMapObjects(LVZLayerType layer);
	void drawMapObjects(double x, double y, double xEnd, double yEnd, LVZLayerType layer);	// starting from x, (lower left corner)
	void drawScreenObjects(LVZLayerType layer);	
	void update(double timestep);				// update animations

private:

	void addImageObject(const string& filename, const LVZImageObject& obj);
	void addMapObject(const LVZMapObject& obj);
	void addScreenObject(const LVZScreenObject& obj);

	void destroy();

	LVZMapObject* findMapObject(int id);
	LVZScreenObject* findScreenObject(int id);

	template <class T>
	bool objHasImage(const T& obj);

	struct LVZItemData
	{
		LVZItemData() : isEnabled(false), displayTimeLeft(0)
		{}

		bool isEnabled;
		double displayTimeLeft;		
	};

private:

	typedef hash_map<int, TextureAnimated> ImageObjectMap;
	typedef hash_multimap<int, LVZMapObject*> MapObjectMap;			// TODO: organize by layer
	typedef hash_multimap<int, LVZScreenObject*> ScreenObjectMap;		// TODO: organize by layer

	ImageObjectMap imageObjects_;
	MapObjectMap mapObjects_[LVZ_NumLayers];			// this is the actual location for the objects, they need to be deleted from here
	ScreenObjectMap screenObjects_[LVZ_NumLayers];

	typedef hash_map<int, LVZItemData> ItemDataMap;
	ItemDataMap itemData_;

	MapObjectMap mapDisplay_;
	ScreenObjectMap screenDisplay_;

	typedef hash_map<int, TextureAnimated> TextureMap;

	TextureManager textureManager_;
	TextureMap textures_;

	SubspaceHud* hud_;
};

#endif