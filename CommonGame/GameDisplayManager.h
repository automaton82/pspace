#ifndef _GAMEDISPLAYMANAGER_H_
#define _GAMEDISPLAYMANAGER_H_

#include <list>
using std::list;

//#include "Database.h"
#include "GameObject.h"
#include "Camera.h"
//#include "Singleton.h"

//assumes random access is not needed, insertion, deletion, and iterative access is most used
//assumes correlation between physical object position and displayed object position

class GameDisplayManager
{
public:

	GameDisplayManager();
	virtual ~GameDisplayManager();

	const Camera& getCamera() const;
	void setCamera(const Camera& camera);

	bool isObjectInView(const GameObject& object) const;

	virtual bool addObject(GameObject* object);
	virtual bool removeObject(GameObject* object);

	//displays all objects set for display
	void lookAtCamera();	//gluLookat for camera
	virtual void display() const;
	
protected:

	virtual void preDisplay() const;	//set up everything for object displays, called by display()

	/*********************************************
	 * NEED TO DO ACTUAL OPTIMIZATION STUFF HERE *
	 *********************************************/
	/*
	//do culling stuff
	void calcFrustum();
	bool pointInFrustum(Pointf p);
	bool sphereInFrustum(Pointf p, double radius);
		
	//do billboarding stuff
	//void rotateTowardsCamera();*/

	typedef list<GameObject*> ObjectList;
	ObjectList objects_;

	Camera camera_;
};

//static GameDisplayManager& gameDisplayManager = *Singleton<GameDisplayManager>::instance();

#endif
