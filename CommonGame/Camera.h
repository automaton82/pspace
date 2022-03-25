#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Positionable.h"

class Camera :
	public Positionable
{
public:
	Camera();
	~Camera();

	const Vector& getViewPoint();
	void setViewPoint(const Vector& viewPoint);
	
private:

	Vector viewPoint;
};

#endif