//David Millman

#ifndef _EXTENDEDWINDOWOBJECT_H_
#define _EXTENDEDWINDOWOBJECT_H_

#include "Color.h"
#include "WindowObject.h"

class ExtendedWindowObject : 
	public WindowObject
{
public:
	ExtendedWindowObject();
	virtual ~ExtendedWindowObject();

	//window properties
	void setBackgroundColor(const Color& c);
	void setBorderWidth(double size);
	void setBorderColor(const Color& c);
	
	//display
	virtual void draw() const;	

	virtual void drawBackground() const;
	virtual void drawBorder() const;

protected:

	//window properties
	Color backgroundColor_;
	Color borderColor_;
	double borderWidth_;
};

#endif