#include "DInputKeyboard.h"

#include <assert.h>

DInputKeyboard::DInputKeyboard(DInputInterface* inputInterface) :
	inputInterface_(inputInterface),
	callbackFunc_(0)
{
	int delay;
	int repeat;

	SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &delay, 0);
	KEY_REPEAT_START = (delay+1)*250;
	SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &repeat, 0);
	KEY_REPEAT_MILLISECONDS = 1000/(repeat+1);
}

DInputKeyboard::~DInputKeyboard()
{
}


void DInputKeyboard::refresh()
{
	InputEventType event;

    inputInterface_->GetKeyStates(keys_);
	
	for(int i=NUM_KEYS-1; i >=0; --i)		//check all keys
	{
		event = this->getInputFromScanCode(i);

		if(KeyDown(keys_,i))
			InputEvent::inputStates[event] = true;
		else if(KeyUp(keys_,i))
			InputEvent::inputStates[event] = false;
		else
			assert(0 && "UNKNOWN KEY STATE");

		if(KeyDown(keys_,i) && !KeyDown(prevKeys_,i))	//key just pressed
		{														
			//InputEvent::inputStates[event] = true;
			(*callbackFunc_)(InputEvent(event, INPUT_STATE_DOWN, INPUT_STATE_UP));	//handle key press
					
			keyRepeatStartTimer_.start();
			keyRepeatTimer_.start();
			repeatScanCode_ = i;
		}
		else if(KeyDown(keys_,i) &&			//key repeat
				(keyRepeatStartTimer_.getElapsedTime() > KEY_REPEAT_START) && 
				(keyRepeatTimer_.getElapsedTime() > KEY_REPEAT_MILLISECONDS) &&
				repeatScanCode_ == i)
		{
			//InputEvent::inputStates[event] = true;
			(*callbackFunc_)(InputEvent(event, INPUT_STATE_DOWN, INPUT_STATE_UP));	//handle key press
            
			keyRepeatTimer_.start();
		}
		else if(KeyUp(keys_,i) && KeyDown(prevKeys_,i))	//key just released
		{		
			//InputEvent::inputStates[event] = false;
			(*callbackFunc_)(InputEvent(event, INPUT_STATE_UP, INPUT_STATE_DOWN));	//handle key press
		}
		else if(KeyDown(keys_,i) && KeyDown(prevKeys_,i))	//key still pressed
		{
			//InputEvent::inputStates[event] = true;
			(*callbackFunc_)(InputEvent(event, INPUT_STATE_DOWN, INPUT_STATE_DOWN));	//handle key press
		}
	}

	memcpy(prevKeys_, keys_, sizeof(char) * NUM_KEYS);
}

void DInputKeyboard::setCallback(CallbackFunc callback)
{
	callbackFunc_ = callback;
}

const char* DInputKeyboard::getAsciiKeyStates()
{
	//TODO: not implemented yet
	return 0;
}

InputEventType DInputKeyboard::getInputFromAscii(char c)
{
	//TODO: not implemented yet
	return (InputEventType)0;
}

InputEventType DInputKeyboard::getInputFromScanCode(int n)
{
	InputEventType input;
	switch(n)
	{
	case DIK_RETURN:	input = KEY_ENTER;		break;

	case DIK_UP:	input = KEY_UP;			break;
	case DIK_DOWN:	input = KEY_DOWN;		break;
	case DIK_LEFT:	input = KEY_LEFT;		break;
	case DIK_RIGHT:	input = KEY_RIGHT;		break;

	case DIK_LSHIFT:	input = KEY_LSHIFT;	break;
	case DIK_RSHIFT:	input = KEY_RSHIFT;	break;
	case DIK_LCONTROL:	input = KEY_LCONTROL;	break;
	case DIK_RCONTROL:	input = KEY_RCONTROL;	break;
	case DIK_TAB:	input = KEY_TAB;		break;

	case DIK_DELETE:	input = KEY_DELETE;		break;
	case DIK_INSERT:	input = KEY_INSERT;		break;
	case DIK_HOME:		input = KEY_HOME;		break;
	case DIK_END:		input = KEY_END;	break;
	case DIK_PGUP:		input = KEY_PAGEUP;	break;
	case DIK_PGDN:		input = KEY_PAGEDOWN;	break;

	case DIK_ESCAPE:	input = KEY_ESCAPE;	break;

	case DIK_F1:	input = KEY_F1;			break;
	case DIK_F2:	input = KEY_F2;			break;
	case DIK_F3:	input = KEY_F3;			break;
	case DIK_F4:	input = KEY_F4;			break;
	case DIK_F5:	input = KEY_F5;			break;
	case DIK_F6:	input = KEY_F6;			break;
	case DIK_F7:	input = KEY_F7;			break;
	case DIK_F8:	input = KEY_F8;			break;
	case DIK_F9:	input = KEY_F9;			break;
	case DIK_F10:	input = KEY_F10;		break;
	case DIK_F11:	input = KEY_F11;		break;
	case DIK_F12:	input = KEY_F12;		break;

	/*case DIK_NUMPADCOMMA:	input = KEY_NUMPADCOMMA;	break;
	case DIK_NUMPADENTER:	input = KEY_NUMPADENTER;	break;
	case DIK_NUMPADEQUALS:	input = KEY_NUMPADEQUALS;	break;
	case DIK_NUMPADMINUS:	input = KEY_NUMPADMINUS;	break;
	case DIK_NUMPADPERIOD:	input = KEY_NUMPADPERIOD;	break;
	case DIK_NUMPADPLUS:	input = KEY_NUMPADPERIOD;	break;
	case DIK_NUMPADSLASH:	input = KEY_NUMPADSLASH;	break;
	case DIK_NUMPADSTAR:	input = KEY_NUMPADSTAR;		break;*/

	default:			//assume ascii value
		input = (InputEventType)(inputInterface_->GetAsciiFromScanCode(n));
	}

	return input;
}