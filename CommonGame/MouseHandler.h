//David Millman
//Computer Graphics
//Lights & Motion

#ifndef _MouseHandler_
#define _MouseHandler_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include "Point.h"

class MouseHandler
{
public:
	MouseHandler();

	void mouseClick(int button, int state, int x, int y);
	void mouseMove(int x, int y);

	int getButtonState(int button) const;
    int getMouseState() const;
	Point getMousePos() const;
	Point getMouseChange() const;
private:

	int mouseState_;
	Point prevMousePos_;
	Point mousePos_;
};

#endif