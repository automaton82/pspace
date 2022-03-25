//David Millman

#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include <vector>
using std::vector;

#include "GameObject.h"
#include "OrthographicObject.h"

//assumes camera is always looking perpendicular at the background, assumes camera z = viewplane (camera is on the viewplane)
//assumes pos z axis is out of the background/out of the screen, y axis is up
//displays objects at default size, for any distance 
//*object size does not scale

class Background :
	public OrthographicObject
{
public:

	Background();			
	virtual ~Background();

	void setAspectRatio(double aspect);		//x:y
	void setFieldOfView(double fov);		//0 < fov < 180, in y axis
	//void setScale(bool b);	//scale object size based on z axis

	void setViewport(double width, double height, double depth = 0);	//TODO: need depth for viewport?

	void setCamera(const Vector& position);
	const Vector& getCamera() const;

	void addObject(GameObject* object);	  //distance in z position of vector
	void clear();

	void calcDepth(double scrollSpeed) const;	//scroll speed 1 = 
	Vector transformPosition(const Vector& pos) const;	//projects x,y,z to x,y
	Vector untransformPosition(const Vector& pos) const;

	void update(double time);

	void draw() const;
	void display() const;

protected:

	typedef vector<GameObject*> GameObjectList;
	
	//view settings
	Vector viewport_;	//width, height, depth - depth is currently ignored
	double fieldOfView_;
	double aspectRatio_;
	double tanThetaX_, tanThetaY_;	//prevent recalculating tan(theta) all the time
	//bool doScale_;

	Vector camera_;

	//objects
	GameObjectList objects_;
	bool isSorted_;
};

#endif