//David Millman
//Computer Graphics

#include "Point.h"

#ifndef _POINTF_H_
#define _POINTF_H_

class Point;

class Pointf
{
public:
	Pointf();
	Pointf(double x, double y, double z);
	Pointf(const Pointf& rhs);
	Pointf(const Point& rhs);

	Pointf& operator=(const Pointf& rhs);

	double x_, y_, z_;
};

#endif