//David Millman
//Computer Graphics
//Lights & Motion

#include "KeyboardHandler.h"

KeyboardHandler::KeyboardHandler()
{
	for(int i=0; i < MAX_KEYS; i++)
	{
		keyStates_[i] = 0;
		specialKeyStates_[i] = 0;
	}
}

void KeyboardHandler::keyDown(unsigned char k, int x, int y)
{
	keyStates_[k] = 1;
}

void KeyboardHandler::keyUp(unsigned char k, int x, int y)
{
	keyStates_[k] = 0;
}

void KeyboardHandler::specialKeyDown(int k, int x, int y)
{
	specialKeyStates_[k] = 1;
}

void KeyboardHandler::specialKeyUp(int k, int x, int y)
{
	specialKeyStates_[k] = 0;
}

const char* KeyboardHandler::getKeyStates() const
{
	return keyStates_;
}

const char* KeyboardHandler::getSpecialKeyStates() const
{
	return specialKeyStates_;
}