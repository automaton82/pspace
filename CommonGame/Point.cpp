//David Millman


#include "Point.h"

Point::Point() : x_(0), y_(0), z_(0)
{
}

Point::Point(int x, int y, int z) : x_(x), y_(y), z_(z)
{
}

Point::Point(const Point& copy)
{
	*this = copy;
}

Point& Point::operator=(const Point& rhs)
{
	if(this != &rhs)
	{
		x_ = rhs.x_;
		y_ = rhs.y_;
		z_ = rhs.z_;
	}
	return *this;
}
