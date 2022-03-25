//David Millman
//Computer Graphics
//Lights & Motion

#ifndef _KeyboardHandler_
#define _KeyboardHandler_

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#define MAX_KEYS 255

class KeyboardHandler
{
public:
	KeyboardHandler();

	void keyDown(unsigned char k, int x, int y);
	void keyUp(unsigned char k, int x, int y);
	void specialKeyDown(int k, int x, int y);
	void specialKeyUp(int k, int x, int y);

	const char* getKeyStates() const;
	const char* getSpecialKeyStates() const;
    
private:
	char keyStates_[MAX_KEYS];
	char specialKeyStates_[MAX_KEYS];
};

#endif