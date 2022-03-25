#ifndef _SORTEDDISPLAYMANAGER_H_
#define _SORTEDDISPLAYMANAGER_H_

#include <map>
using std::multimap;

#include "GameDisplayManager.h"

//assumes sorting based on z-position
//sorts display objects based on depth (for rendering order)

class SortedDisplayManager :
	public GameDisplayManager
{
public:

	SortedDisplayManager();
	~SortedDisplayManager();

	bool addObject(double key, GameObject* object);	//sort key = z-coord typically, for display layers
	bool addObject(GameObject* object);		//automatically grabs z-coordinate
	bool removeObject(GameObject* object);

	void display() const;		//displays all object in ascending key order, -n...m

private:

	typedef multimap<double, GameObject*> SortedObjectList;
	SortedObjectList sortedObjects_;
};

//static GameDisplayManager& gameDisplayManager = *Singleton<GameDisplayManager>::instance();

#endif
