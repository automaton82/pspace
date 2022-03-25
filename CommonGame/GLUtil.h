#ifndef _GLUTIL_H_
#define _GLUTIL_H_

#include <windows.h>

#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

#include "Vector.h"

namespace GLUtil
{
	void drawUnitQuad();	//draws in x,y plane
	void drawQuad(double x1, double y1, double x2, double y2);
	void drawQuad(double width, double height);

	void drawLineRect(double width, double height);	//draws in x,y plane - draws a rectangle from lines
    
	//TODO: color gradient stuff?
	//setColorGradientLinear(double time, double period, const Color& start, const Color& end);

	static double modelviewMatrix[16], projectionMatrix[16];
	static int viewportMatrix[4];
	
	void setProjectionMatrices();	//store the modelview, projection, and viewport matrices for use later
	void projectPoint(double x, double y, double z, double* wx, double* wy, double* wz);	//object to window coords
	void unprojectPoint(double wx, double wy, double wz, double *x, double* y, double* z);	//window to object coords

	void getViewVector(double wx, double wy, double* x0, double* y0, double* z0, double* xd, double* yd, double* zd);	//given window coordinates, get ray origin and direction
	void getViewVector(double wx, double wy, Vector* viewOrigin, Vector* viewDirection);

	void setOrthoMode(double x, double y, double width, double height, double nearClip = -1000, double farClip = 1000);
	void enterOrthoMode();
	void exitOrthoMode();

	void saveColor();
	void restoreColor();
}

#endif