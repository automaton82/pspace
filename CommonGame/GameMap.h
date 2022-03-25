//David Millman

#include "Database.h"
#include "Updatable.h"

#include "GameObject.h"

#ifndef _GAMEMAP_H_
#define _GAMEMAP_H_

class GameMap :
	public GameObject,
	public Database<GameObject>
{
public:
	//GameMap();
	virtual ~GameMap();

	//void update(double timeMultiplier);

	//MUST IMPLEMENT THIS
	//does only drawing associated with the actual object (i.e. no glTranslate for position)
	void draw() const;

protected:
};

#endif