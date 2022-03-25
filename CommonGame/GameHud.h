//David Millman

#ifndef _GAMEHUD_H_
#define _GAMEHUD_H_

#include "OrthographicObject.h"
#include "Database.h"

class GameObject;

class GameHud :
	public OrthographicObject
{
public:

	virtual ~GameHud();

	int add(GameObject* object);

	virtual void update(double timeMultiplier);

	virtual void draw() const;

	virtual void display() const;	//takes care of orthographic projection stuff

private:
	
	Database<GameObject> objects;
};

#endif