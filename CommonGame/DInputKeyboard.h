#ifndef _DINPUTKEYBOARD_H_
#define _DINPUTKEYBOARD_H_

#include "DInputInterface.h"
#include "Timer.h"
#include "InputEvent.h"


//keyboard wrapper for DInputInterface; this probably should be its own class
class DInputKeyboard
{
public:

	typedef void(*CallbackFunc)(InputEvent event);			

public:
	DInputKeyboard(DInputInterface* inputInterface);
	~DInputKeyboard();

	void refresh();

	//TODO: make callback use KeyboardEventType
	void setCallback(CallbackFunc callback);	//calls back with InputEvent

	const char* getKeyStates();
	void getKeyStates(char* keyState);	//256 chars	
	const char* getAsciiKeyStates();
   
	InputEventType getInputFromAscii(char c);
	InputEventType getInputFromScanCode(int n);

private:
	DInputInterface* inputInterface_;

	Timer keyRepeatTimer_;
	Timer keyRepeatStartTimer_;
	int repeatScanCode_;

	CallbackFunc callbackFunc_;

	static const int NUM_KEYS = 256;
	int KEY_REPEAT_START;
	int KEY_REPEAT_MILLISECONDS;

	char keys_[NUM_KEYS];		//current key state
	char prevKeys_[NUM_KEYS];	//previous key state
	
};

#endif
