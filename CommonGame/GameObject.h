//David Millman

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <assert.h>

#include "Object.h"
#include "Displayable.h"
#include "Positionable.h"
#include "Rotatable.h"
#include "Sizable.h"
#include "Updatable.h"

class GameObject :
	public Object,
	public Displayable,
	public Positionable,
	public Rotatable,
	public Sizable,
	public Updatable
{
public:
	GameObject();
	virtual ~GameObject();

	bool isGarbage() const;
	void setGarbage(bool b);

	//Shold implement this
	virtual void update(double timeMultiplier);

	//MUST IMPLEMENT THIS
	//does only drawing associated with the actual object (i.e. no glTranslate for position)
	virtual void draw() const = 0;

	//does everything necessary to display an object
	virtual void display() const;

protected:

	bool garbage;
};

#endif