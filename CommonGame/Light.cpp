//David Millman
//Computer Graphics

#include "Light.h"

const double Positional = 0.0;

Light::Light(int lightNum) : lightNum_(lightNum)
{
	float pArray[4] = {0,0,0,Positional};
	glLightfv(lightNum_, GL_POSITION, pArray);
}

void Light::enable()
{
	glEnable(lightNum_);
}

void Light::disable()
{
	glDisable(lightNum_);
}

void Light::setPosition(double x, double y, double z)
{
	float pArray[4] = {x,y,z, 1};
	position_ = Pointf(x,y,z);

    glLightfv(lightNum_, GL_POSITION, pArray);
}

void Light::recalcPosition()
{
	float pArray[4] = {position_.x_, position_.y_, position_.z_, 1};
	
	glLightfv(lightNum_, GL_POSITION, pArray);
}

void Light::setAmbient(double r, double g, double b)
{
	ambient_[0] = r;
	ambient_[1] = g;
	ambient_[2] = b;
	ambient_[3] = 1;

	glLightfv(lightNum_, GL_AMBIENT, ambient_);
}

void Light::setDiffuse(double r, double g, double b)
{
	diffuse_[0] = r;
	diffuse_[1] = g;
	diffuse_[2] = b;
	diffuse_[3] = 1;

	glLightfv(lightNum_, GL_DIFFUSE, diffuse_);
}

void Light::setSpecular(double r, double g, double b)
{
	specular_[0] = r;
	specular_[1] = g;
	specular_[2] = b;
	specular_[3] = 1;

	glLightfv(lightNum_, GL_SPECULAR, specular_);
}

void Light::setAttenuation(double att)
{
	glLightf(lightNum_, GL_QUADRATIC_ATTENUATION, att);
}

void Light::setCutoff(double angle)
{
	cutoff_ = angle;
	glLightf(lightNum_, GL_SPOT_CUTOFF, angle);
	glLightf(lightNum_, GL_SPOT_EXPONENT, 3);
//	glLightf(lightNum_, GL_CONSTANT_ATTENUATION, 2.0);
//	glLightf(lightNum_, GL_LINEAR_ATTENUATION, 0.1);
//glLightf(lightNum_, GL_QUADRATIC_ATTENUATION, 0.0005);
}

void Light::setDirection(double x, double y, double z)
{
	float dirArray[4] = {x, y, z, 1};
	direction_ = Pointf(x, y, z);
	glLightfv(lightNum_, GL_SPOT_DIRECTION, dirArray);
}

Pointf Light::getPosition()
{
	return position_;
}