#include "MathUtil.h"

#include <math.h>
#include <stdlib.h>	//for rand

namespace MathUtil
{
	double round(double value)
	{
		double retval = value;

		double roundedLow = ::floor(value);
		double roundedHigh = ::ceil(value);

		double absDiffHigh = fabs(value - roundedHigh);
		double absDiffLow = fabs(value - roundedLow);

		if(absDiffLow < absDiffHigh)
			retval = roundedLow;
		else
			retval = roundedHigh;

		return retval;
	}

	double round(double value, double delta)
	{
		double retval = value;

		double roundedLow = value - fmod(value, delta);
		double roundedHigh = roundedLow + delta;

		double absDiffHigh = fabs(value - roundedHigh);
		double absDiffLow = fabs(value - roundedLow);

		if(absDiffLow < absDiffHigh)
			retval = roundedLow;
		else
			retval = roundedHigh;

		return retval;
	}

	double floor(double value, double delta)
	{
		if(value >= 0)
			return value - fmod(value, delta);
		else
			return value - delta - fmod(value, delta);
				
		//double retval = value - fmod(value, delta);
		//return retval;
	}

	double ceil(double value, double delta)
	{
		if(value >= 0)
			return value + delta - fmod(value, delta);
		else
			return value - fmod(value, delta);

		//double retval = value - fmod(value), delta);
		//retval += delta;

		//return retval;
	}

	double clamp(double value, double epsilon)
	{
		double retval = value;

		double roundedHigh = ::ceil(value);
		double roundedLow = ::floor(value);

		double absDiffHigh = fabs(value - roundedHigh);
		double absDiffLow = fabs(value - roundedLow);

		if(absDiffLow < absDiffHigh && 
			absDiffLow < epsilon)
		{
			//use roundedLow
			retval = roundedLow;
		}
		else if(absDiffHigh < epsilon)
		{
			retval = roundedHigh;
		}
			
		return retval;
	}

	bool pointInBox(double x, double y, const Rect& box)
	{
		//TODO: check top and bottom
		if(x < box.left || x > box.right ||
			y < box.bottom || y > box.top)
			return false;
		else
			return true;
	}

	double unitRand()
	{
		double retval = rand() / (double)RAND_MAX;
		return retval;
	}

	double unitRandDiscreet(unsigned int intervals)
	{
		double retval = ::floor(unitRand()*intervals) / (double)intervals;
		return retval;
	}

	Vector rayPlaneIntersection(const Vector& rayOrigin, const Vector& rayDirection, const Vector& planeNormal, double planeDistance)
	{
			//ray equation: R = R0 + tD
		Vector pointOnPlane = planeNormal * planeDistance;	//TODO: this may not be a valid point
		double t = - ((rayOrigin-pointOnPlane).dot(planeNormal)+planeDistance) / rayDirection.dot(planeNormal);

		//TODO: check for special cases

		Vector point = rayOrigin + t*rayDirection;	//intersection
		return point;
	}

	Vector rotateVectorX(const Vector& v, double angle)
	{
		return Vector(	v.x_,
						v.y_*cos(angle) - v.z_*sin(angle),
						v.y_*sin(angle) - v.z_*cos(angle) );
	}

	Vector rotateVectorY(const Vector& v, double angle)
	{
		return Vector(	v.x_*cos(angle) - v.z_*sin(angle),
						v.y_,
						-v.x_*sin(angle) + v.z_*cos(angle) );
	}

	Vector rotateVectorZ(const Vector& v, double angle)
	{
		return Vector(	v.x_*cos(angle) - v.y_*sin(angle),
						v.x_*sin(angle) + v.y_*cos(angle),
						v.z_ );
	}

}	//namespace MathUtil