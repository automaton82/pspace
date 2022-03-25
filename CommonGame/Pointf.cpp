//David Millman
//Computer Graphics

#include "Pointf.h"

Pointf::Pointf() : x_(0), y_(0), z_(0)
{
}

Pointf::Pointf(double x, double y, double z) : x_(x), y_(y), z_(z)
{
}

Pointf::Pointf(const Pointf& copy)
{
	*this = copy;
}

Pointf::Pointf(const Point& copy)
{
	x_ = copy.x_;
	y_ = copy.y_;
	z_ = copy.z_;
}

Pointf& Pointf::operator=(const Pointf& rhs)
{
	if(this != &rhs)
	{
		x_ = rhs.x_;
		y_ = rhs.y_;
		z_ = rhs.z_;
	}
	return *this;
}