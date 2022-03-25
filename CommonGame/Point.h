//David Millman

#ifndef _POINT_H_
#define _POINT_H_

class Point
{
public:
	Point();
	Point(int x, int y, int z);
	Point(const Point& rhs);

	Point& operator=(const Point& rhs);

	int x_, y_, z_;
};

struct Point2
{
	Point2() : x(0), y(0)
	{}

	Point2(double pX, double pY) : x(pX), y(pY)
	{}

	double x, y;
};

struct Point3
{
	Point3() : x(0), y(0)
	{}

	Point3(double pX, double pY, double pZ) : x(pX), y(pY), z(pZ)
	{}

	double x, y, z;
};


#endif