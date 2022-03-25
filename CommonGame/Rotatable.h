#ifndef _ROTATEABLE_H_
#define _ROTATEABLE_H_

#include <string>
#include <math.h>
#include "Vector.h"

#include "MathUtil.h"

const static double DEGREES_TO_RADIANS = MathUtil::PI / 180.0f;
const static double RADIANS_TO_DEGREES = 180.0f / MathUtil::PI;

using std::string;

class Rotatable
{
public:

	Rotatable();
	virtual ~Rotatable();

	virtual const Vector& getDirection() const;

	virtual void setDirection(const Vector& direction);
	
	//angle in degrees
	virtual void rotateX(double angle);
	virtual void rotateY(double angle);
	virtual void rotateZ(double angle);
	
protected:
	Vector direction_;
};

#endif