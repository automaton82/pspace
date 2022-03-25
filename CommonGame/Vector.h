//David Millman

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <math.h>
#include <stdio.h>

//TODO: make quaternion class

class Vector
{
public:
	Vector();
	Vector(double x, double y, double z);

	Vector& operator +=(const Vector& rhs);
	Vector& operator -=(const Vector& rhs);
	Vector& operator *=(const Vector& rhs);
	Vector& operator /=(const Vector& rhs);

	Vector& operator +=(const double rhs);
	Vector& operator -=(const double rhs);
	Vector& operator *=(const double rhs);
	Vector& operator /=(const double rhs);

	Vector operator -();
	
	friend Vector operator +(const Vector& lhs, const Vector& rhs);
	friend Vector operator -(const Vector& lhs, const Vector& rhs);
	friend Vector operator *(const Vector& lhs, const Vector& rhs);
	friend Vector operator /(const Vector& lhs, const Vector& rhs);

	friend Vector operator +(const Vector& lhs, const double rhs);
	friend Vector operator -(const Vector& lhs, const double rhs);
	friend Vector operator *(const Vector& lhs, const double rhs);
	friend Vector operator /(const Vector& lhs, const double rhs);

	friend Vector operator +(const double lhs, const Vector& rhs);
	//friend Vector operator -(const double lhs, const Vector& rhs);
	friend Vector operator *(const double lhs, const Vector& rhs);
	//friend Vector operator /(const double lhs, const Vector& rhs);
	
	double magnitude() const;
	Vector unitVector() const;
	void normalize();

	double dot(const Vector& rhs) const;
	Vector cross(const Vector& rhs) const;

	void rotateX(double angle);		//angle in radians
	void rotateY(double angle);
	void rotateZ(double angle);

	void clear();
	void zero();

	double x_, y_, z_;
};

Vector abs(const Vector& v);

#endif