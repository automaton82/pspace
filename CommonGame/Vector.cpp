//David Millman


#include "Vector.h"

Vector::Vector() : x_(0), y_(0), z_(0)
{
}

Vector::Vector(double x, double y, double z) : x_(x), y_(y), z_(z)
{
}

Vector Vector::operator -()
{
	//prfloatf("vector negation\n");
	return Vector(-x_, -y_, -z_);
}

Vector& Vector::operator +=(const Vector& rhs)
{
	x_ += rhs.x_;
	y_ += rhs.y_;
	z_ += rhs.z_;
	
	return *this;
}

Vector& Vector::operator -=(const Vector& rhs)
{
	x_ -= rhs.x_;
	y_ -= rhs.y_;
	z_ -= rhs.z_;

	return *this;
}

Vector& Vector::operator *=(const Vector& rhs)
{
	x_ *= rhs.x_;
	y_ *= rhs.y_;
	z_ *= rhs.z_;

	return *this;
}

Vector& Vector::operator /=(const Vector& rhs)
{
	x_ /= rhs.x_;
	y_ /= rhs.y_;
	z_ /= rhs.z_;

	return *this;
}


Vector& Vector::operator +=(const double rhs)
{
	x_ += rhs;
	y_ += rhs;
	z_ += rhs;

	return *this;
}

Vector& Vector::operator -=(const double rhs)
{
	x_ -= rhs;
	y_ -= rhs;
	z_ -= rhs;

	return *this;
}

Vector& Vector::operator *=(const double rhs)
{
	x_ *= rhs;
	y_ *= rhs;
	z_ *= rhs;

	return *this;
}

Vector& Vector::operator /=(const double rhs)
{
	x_ /= rhs;
	y_ /= rhs;
	z_ /= rhs;

	return *this;
}

Vector operator +(const Vector& lhs, const Vector& rhs)
{
 	Vector v(lhs.x_+rhs.x_, lhs.y_+rhs.y_, lhs.z_+rhs.z_);
	return v;
}

Vector operator -(const Vector& lhs, const Vector& rhs)
{
	Vector v(lhs.x_-rhs.x_, lhs.y_-rhs.y_, lhs.z_-rhs.z_);
	return v;
}

Vector operator *(const Vector& lhs, const Vector& rhs)
{
	Vector v(lhs.x_*rhs.x_, lhs.y_*rhs.y_, lhs.z_*rhs.z_);
	return v;
}

Vector operator /(const Vector& lhs, const Vector& rhs)
{
	Vector v(lhs.x_/rhs.x_, lhs.y_/rhs.y_, lhs.z_/rhs.z_);
	return v;
}

Vector operator +(const Vector& lhs, const double rhs)
{
	Vector v(lhs.x_+rhs, lhs.y_+rhs, lhs.z_+rhs);
	return v;
}

Vector operator -(const Vector& lhs, const double rhs)
{
	Vector v(lhs.x_-rhs, lhs.y_-rhs, lhs.z_-rhs);
	return v;
}

Vector operator *(const Vector& lhs, const double rhs)
{
	Vector v(lhs.x_*rhs, lhs.y_*rhs, lhs.z_*rhs);
	return v;
}

Vector operator /(const Vector& lhs, const double rhs)
{
	Vector v(lhs.x_/rhs, lhs.y_/rhs, lhs.z_/rhs);
	return v;
}

Vector operator +(const double lhs, const Vector& rhs)
{
	Vector v(lhs+rhs.x_, lhs+rhs.y_, lhs+rhs.z_);
	return v;
}

//this isn't quite right
/*Vector operator -(const double lhs, const Vector& rhs)
{
	Vector v(lhs-rhs.x_, lhs-rhs.y_, lhs-rhs.z_);
	return v;
}*/

Vector operator *(const double lhs, const Vector& rhs)
{
	Vector v(lhs*rhs.x_, lhs*rhs.y_, lhs*rhs.z_);
	return v;
}

//this isn't quite right either
/*Vector operator /(const double lhs, const Vector& rhs)
{
	Vector v(lhs*rhs.x_, lhs*rhs.y_, lhs*rhs.z_);
	return v;
}*/

double Vector::magnitude() const
{
	return sqrt(x_*x_ + y_*y_ + z_*z_);
}

double Vector::dot(const Vector& rhs) const
{
	return (x_*rhs.x_ + y_*rhs.y_ + z_*rhs.z_);
}

Vector Vector::cross(const Vector& rhs) const
{
	return Vector(
		y_*rhs.z_ - z_*rhs.y_, 
		z_*rhs.x_ - x_*rhs.z_,
		x_*rhs.y_ - y_*rhs.x_
		);
}

Vector Vector::unitVector() const
{
	if(magnitude() == 0) return *this;
	return (*this / magnitude());
}

void Vector::normalize()
{
	double m = magnitude();
	if(m == 0)
		x_ = y_ = z_ = 0;
	else
		(*this) /= m;
}

Vector abs(const Vector& v)
{
	return Vector(
		fabs(v.x_),
		fabs(v.y_),
		fabs(v.z_)
		);
}

void Vector::rotateX(double angle)
{
	double y = y_;
	double z = z_;

	y_ = y*cos(angle) - z*sin(angle);
	z_ = y*sin(angle) - z*cos(angle);
}

void Vector::rotateY(double angle)
{
	double x = x_;
	double z = z_;

	x_ = x*cos(angle) - z*sin(angle);
	z_ = -x*sin(angle) + z*cos(angle);
}

void Vector::rotateZ(double angle)
{
	double x = x_;
	double y = y_;
	
	x_ = x*cos(angle) - y*sin(angle);
	y_ = x*sin(angle) + y*cos(angle);
}

void Vector::clear()
{
	x_ = y_ = z_ = 0;
}

void Vector::zero()
{
	x_ = y_ = z_ = 0;
}
