//David Millman
//Computer Graphics

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <GL/glut.h>

#include "Pointf.h"

/**
* A wrapper class to handle openGL lights. 
*
* @author	Dave Millman (millmd@rpi.edu)
*/
class Light
{
public:
	Light(int lightNum);
	
	void enable();
	void disable();

	void setPosition(double x, double y, double z);
	void recalcPosition();

	void setAmbient(double r, double g, double b);
	void setDiffuse(double r, double g, double b);
	void setSpecular(double r, double g, double b);

	void setCutoff(double angle);
	void setDirection(double x, double y, double z);
	void setAttenuation(double att);

	Pointf getPosition();
	
private:
	int lightNum_;

	double cutoff_;

	float ambient_[4];
	float diffuse_[4];
	float specular_[4];

	Pointf direction_;
	Pointf position_;
};

#endif