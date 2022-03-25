//David Millman
//Computer Graphics

#include "Material.h"

#include <windows.h>
#include <gl/gl.h>

Material::Material()
{
	ambient_[0] = ambient_[1] = ambient_[2] = 0.2f;
	ambient_[3] = 1;

	diffuse_[0] = diffuse_[1] = diffuse_[2] = 0.8f;
	diffuse_[3] = 1;		

	for(int i=0; i < 4; i++)
	{
		specular_[i] = 0;
		emission_[i] = 0;
	}
	shininess_ = 0;

	alpha_ = 1.0;
}

Material::Material(const Color& c)
{
	setColor(c);
}

void Material::enable()
{

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess_);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission_);
}

void Material::disable()
{
	float empty[4] = {0,0,0,0};

	glMaterialfv(GL_FRONT, GL_AMBIENT, empty);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, empty);
	glMaterialfv(GL_FRONT, GL_SPECULAR, empty);
	glMaterialfv(GL_FRONT, GL_SHININESS, empty);
	glMaterialfv(GL_FRONT, GL_EMISSION, empty);
}

void Material::setColor(const Color& c, double a, double d, double s)
{
	//double a = 0.5;	//ambient scale
	//double d = 1.0;	//diffuse scale
	//double s = 0.4;	//specular scale

	setAmbient(c.r*a, c.g*a, c.b*a);
	setDiffuse(c.r*d, c.g*d, c.b*d);
	setSpecular(c.r*s, c.g*s, c.b*s);
	setAlpha(c.a);
}

void Material::setColor(const string& s)
{

}

void Material::setAmbient(double r, double g, double b)
{
	ambient_[0] = (float)r;
	ambient_[1] = (float)g;
	ambient_[2] = (float)b;
	ambient_[3] = alpha_;

	//glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_);
}

void Material::setDiffuse(double r, double g, double b)
{
	diffuse_[0] = (float)r;
	diffuse_[1] = (float)g;
	diffuse_[2] = (float)b;
	diffuse_[3] = alpha_;

	//glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_);
}

void Material::setSpecular(double r, double g, double b)
{
	specular_[0] = (float)r;
	specular_[1] = (float)g;
	specular_[2] = (float)b;
	specular_[3] = alpha_;

	//glMaterialfv(GL_FRONT, GL_SPECULAR, specular_);
}

void Material::setShininess(double s)
{
	shininess_ = (float)s;
	
	//glMaterialf(GL_FRONT, GL_SHININESS, shininess_);
}

void Material::setEmission(double r, double g, double b)
{
	emission_[0] = (float)r;
	emission_[1] = (float)g;
	emission_[2] = (float)b;
	emission_[3] = alpha_;

	//glMaterialfv(GL_FRONT, GL_EMISSION, emission_);
}

void Material::setAlpha(double a)
{
	alpha_ = (float)a;

	ambient_[3] = (float)alpha_;
	diffuse_[3] = (float)alpha_;
	specular_[3] = (float)alpha_;
	emission_[3] = (float)alpha_;
}