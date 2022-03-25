#ifndef _MATERIAL_H_
#define _MATERIAL_H_

//#include <stdlib.h>
//#include <stdio.h>
//#include <GL/glut.h>

//#include <windows.h>
//#include <gl/glut.h>

#include <string>
using std::string;

#include "Color.h"

/**
* A wrapper class to handle openGL lights. 
*
* @author	Dave Millman (millmd@rpi.edu)
*/

class Material
{
public:
	Material();
	Material(const Color& c);
	
	void enable();
	void disable();

	void setColor(const Color& c, double ambientScale = 0.5, double diffuseScale = 1.0, double specularScale = 0.4);
	void setColor(const string& s);		//TODO: implement color based on name

	void setAmbient(double r, double g, double b);
	void setDiffuse(double r, double g, double b);
	void setSpecular(double r, double g, double b);
	void setShininess(double s);
	void setEmission(double r, double g, double b);

	void setAlpha(double a);
private:
	
	float ambient_[4];
	float diffuse_[4];
	float specular_[4];
	float shininess_;
	float emission_[4];

	float alpha_;
};

#endif