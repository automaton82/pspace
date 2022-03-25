#ifndef _DINPUTMOUSE_H_
#define _DINPUTMOUSE_H_

#include "DInputInterface.h"
#include "InputEvent.h"


//mouse wrapper for DInputInterface; this probably should be its own class
class DInputMouse
{
public:

	typedef void(*CallbackFunc)(InputEvent event, long mouseX, long mouseY, long mouseZ);			

public:
	DInputMouse(DInputInterface* inputInterface, bool useCallbackPassive = false);
	~DInputMouse();

	void refresh();

	//TODO: make callback use KeyboardEventType
	void setCallback(CallbackFunc callback);	//calls back with InputEvent

	const char* getKeyStates();
	void getKeyStates(char* keyState);	//4 chars	
	   
	InputEventType getInputFromScanCode(int n);

private:
	DInputInterface* inputInterface_;

	CallbackFunc callbackFunc_;

	static const int NUM_BUTTONS = 4;

	char buttons_[NUM_BUTTONS];		//current button state
	char prevButtons_[NUM_BUTTONS];	//previous button state

	DIMOUSESTATE mouse_;
	DIMOUSESTATE prevMouse_;

	bool useCallbackPassive_;		// should it callback on relative mouse movements?
};

#endif
