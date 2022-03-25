
#include "SortedDisplayManager.h"

#include "GLObject.h"


SortedDisplayManager::SortedDisplayManager()
{
}

SortedDisplayManager::~SortedDisplayManager()
{
}

bool SortedDisplayManager::addObject(GameObject* object)
{
	assert(object);

	double key = object->getPosition().z_;

	return addObject(key, object);
}

bool SortedDisplayManager::addObject(double key, GameObject* object)
{
	assert(object);

	sortedObjects_.insert(std::pair<SortedObjectList::key_type, SortedObjectList::mapped_type>(key, object));
	//sortedObjects_.insert(SortedObjectList::pair(key, object));
	
	return true;
}

bool SortedDisplayManager::removeObject(GameObject* object)
{
	objects_.remove(object);

	return true;
}

void SortedDisplayManager::display() const
{
	glPushMatrix();

		preDisplay();

		SortedObjectList::const_iterator i;
		for(i = sortedObjects_.begin(); i != sortedObjects_.end(); ++i)
		{
			(*i).second->display();
		}
	
	glPopMatrix();
}