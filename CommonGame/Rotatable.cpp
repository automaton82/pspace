#include "Rotatable.h"

Rotatable::Rotatable()
{
	setDirection(Vector(1, 0, 0));

}

Rotatable::~Rotatable()
{
}

const Vector& Rotatable::getDirection() const
{
	return direction_;
}

void Rotatable::setDirection(const Vector& d)
{
	this->direction_ = d;
}

void Rotatable::rotateX(double angle)
{
	double y = direction_.y_;
	double z = direction_.z_;

	//this->direction_.x_ = x;
	direction_.y_ = y*cos(angle) - z*sin(angle);
	direction_.z_ = y*sin(angle) - z*cos(angle);
}

void Rotatable::rotateY(double angle)
{
	double x = direction_.x_;
	//double y = direction__.y_;
	double z = direction_.z_;

	direction_.x_ = x*cos(angle) - z*sin(angle);
	//direction__.y_ = y;
	direction_.z_ = -x*sin(angle) + z*cos(angle);
}

void Rotatable::rotateZ(double angle)
{
	double x = direction_.x_;
	double y = direction_.y_;
	//double z = direction__.z_;

	angle *= DEGREES_TO_RADIANS;

	direction_.x_ = x*cos(angle) - y*sin(angle);
	direction_.y_ = x*sin(angle) + y*cos(angle);
	//direction__.z_ = z;
}
