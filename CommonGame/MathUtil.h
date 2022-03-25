#ifndef _MATHUTIL_H_
#define _MATHUTIL_H_

#include "Rect.h"
#include "Vector.h"

#include <math.h>

namespace MathUtil
{
	const double pi = 3.14159265358979323846;
	const double PI = 3.14159265358979323846;

	template <class T>
	inline T degreesToRadians(T degrees)
	{
		return (degrees * pi/180.0);
	}

	template <class T>
	inline T radiansToDegrees(T radians)
	{
		return (radians * 180/pi);
	}

	/********************************
	  Description:
		projects x,y to y coordinate
	  Parameters:					
	    h- offset from view center 
	    yTotal- viewplane height
	    z- distance from viewplane
		tanTheta- tan of field of view (field of view is actually 2*theta), in radians
	*********************************/
	template <class T>
	T projectPoint(T h, T yTotal, T z, T tanTheta)		//error corrected: atanTheta changed to tanTheta
	{
		return ( yTotal/(yTotal + 2*z*tanTheta) * h ); 
	}

	template <class T>
	T unprojectPoint(T hPrime, T yTotal, T z, T tanTheta)
	{
		return ( hPrime * (yTotal + 2*z*tanTheta) / yTotal );
	}

	template <class T>
	T sign(T value)
	{
		if(value >= 0)
			return 1;
		else 
			return -1;
	}

	//TODO: is there an already existing rounding function?
	//TODO: these are not robustly tested
	double round(double value);		//round to nearest integer
	double round(double value, double delta);	//round to nearest delta

	double floor(double value, double delta);	//round down to nearest delta
	double ceil(double value, double delta);	//round up to nearest delta

	double clamp(double value, double epsilon);	//clamps to nearest integer if within epsilon
	
	//TODO: distinction between points and vectors?
	bool pointInBox(double x, double y, const Rect& box);

	double unitRand();	//random number from [0,1)
	double unitRandDiscreet(unsigned int intervals);	//random number from [0,1) in specified intervals

	//geometry functions
	Vector rayPlaneIntersection(const Vector& rayOrigin, const Vector& rayDirection, const Vector& planeNormal, double planeDistance);


	//interpolation
	template <class T>
	T linearInterp(const T& a, const T& b, double weight)
	{
		return ( (1.0-weight)*a + (weight)*b );
	}

	Vector rotateVectorX(const Vector& v, double angle);
	Vector rotateVectorY(const Vector& v, double angle);
	Vector rotateVectorZ(const Vector& v, double angle);
}	//namespace MathUtil

#endif