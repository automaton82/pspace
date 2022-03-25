//David Millman

#ifndef _ORTHOGRAPHICOBJECT_H_
#define _ORTHOGRAPHICOBJECT_H_

#include "Displayable.h"
#include "Updatable.h"

class OrthographicObject :
	public Displayable,
	public Updatable
{
public:

	OrthographicObject();
	virtual ~OrthographicObject();

	virtual void update(double time) = 0;

	virtual void draw() const = 0;

	//x,y at lower left
	void setOrthoMode(double x, double y, double width, double height, double nearClip = -1000, double farClip = 1000);
	void enterOrthoMode() const;
	void exitOrthoMode() const;

		//TODO: make an orthographic display manager that takes care of orthographic projection
	virtual void display() const;	//takes care of orthographic projection stuff

private:
	
	double orthoX_, orthoY_, orthoWidth_, orthoHeight_, orthoNear_, orthoFar_;
};

#endif