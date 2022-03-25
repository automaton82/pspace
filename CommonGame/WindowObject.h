//David Millman

#ifndef _WINDOWOBJECT_H_
#define _WINDOWOBJECT_H_

#include <list>
using std::list;

#include "InputEvent.h"
#include "OrthographicObject.h"

class WindowObject
{
public:
	WindowObject();
	virtual ~WindowObject();

	//double getX() const;
	//double getY() const;
	double getHeight() const;
	double getWidth() const;

	//window functions
	virtual bool handleDragInput(InputEvent input, double x0, double y0, double x1, double y1);
	virtual bool handleInput(InputEvent input, double x, double y);
	//virtual bool move(double x, double y);
	virtual void onResize();
	virtual bool resize(double width, double height);	
	virtual void onSelect();

	//void display() const;
	virtual void draw() const;	
	virtual void update(double time);	//for animated windows and such

protected:

	bool isInBounds(double x, double y) const;
	//list<WindowObject> WindowList;
	
protected:

	//double resizeAreaSize_;	//area that a resize is selected

	//double x_, y_;				//position
	double width_, height_;		//size

	//WindowList windows_;
};

#endif