//David Millman

#ifndef _MENUOBJECT_H_
#define _MENUOBJECT_H_

#include "Functor.h"
#include "ExtendedWindowObject.h"

class MenuObject : 
	public ExtendedWindowObject
{
public:

	typedef Functor0 MenuCallback;

public:
	MenuObject(double width=0, double height=0);
	virtual ~MenuObject();

	void setCallback(MenuCallback* callback);

	//window functions
	virtual bool handleInput(InputEvent input, double x, double y);

	virtual void onHover();		//mouse is hovering over menu object
	virtual void onSelect();	//e.g. left click, calls callback
	
	virtual void draw() const;	
	virtual void update(double time);	//for animated windows and such
	
protected:

	MenuCallback* callback_;
};

#endif