#include "DInputMouse.h"

#include <assert.h>

DInputMouse::DInputMouse(DInputInterface* inputInterface, bool useCallbackPassive) :
	inputInterface_(inputInterface),
	callbackFunc_(0),
	useCallbackPassive_(useCallbackPassive)
{
	memset(&mouse_, 0, sizeof(mouse_));
	memset(&prevMouse_, 0, sizeof(prevMouse_));
}

DInputMouse::~DInputMouse()
{
}


void DInputMouse::refresh()
{
	InputEventType event;

	if(!callbackFunc_)
		return;

	mouse_ = *inputInterface_->GetMouseState();

	
	for(int i=0; i < NUM_BUTTONS; ++i)		//check all keys
	{
		event = this->getInputFromScanCode(i);

		if(KeyDown(mouse_.rgbButtons, i))
			InputEvent::inputStates[event] = true;
		else if(KeyUp(mouse_.rgbButtons, i))
			InputEvent::inputStates[event] = false;		
		else
			assert(0 && "UNKNOWN BUTTON STATE");

		if(KeyDown(mouse_.rgbButtons,i) && KeyUp(prevMouse_.rgbButtons,i))	//key just pressed
		{
			(*callbackFunc_)(InputEvent(event, INPUT_STATE_DOWN, INPUT_STATE_UP), mouse_.lX, mouse_.lY, mouse_.lZ);	
		}
		else if(KeyUp(mouse_.rgbButtons,i) && KeyDown(prevMouse_.rgbButtons,i))	//key just released
		{
			(*callbackFunc_)(InputEvent(event, INPUT_STATE_UP, INPUT_STATE_DOWN), mouse_.lX, mouse_.lY, mouse_.lZ);	
		}
		else if(KeyDown(mouse_.rgbButtons,i) && KeyDown(prevMouse_.rgbButtons,i))	//key still pressed
		{
			(*callbackFunc_)(InputEvent(event, INPUT_STATE_DOWN, INPUT_STATE_DOWN), mouse_.lX, mouse_.lY, mouse_.lZ);	
		}		
	}

	if(useCallbackPassive_ && 
			(mouse_.lX != 0	||				// these are relative changes
			 mouse_.lY != 0 ||
			 mouse_.lZ != 0)	)
	{
		(*callbackFunc_)(InputEvent(MOUSE_PASSIVE, INPUT_STATE_UNKNOWN, INPUT_STATE_UNKNOWN), mouse_.lX, mouse_.lY, mouse_.lZ);	
	}

	prevMouse_ = mouse_;	
}

void DInputMouse::setCallback(CallbackFunc callback)
{
	callbackFunc_ = callback;
}

InputEventType DInputMouse::getInputFromScanCode(int n)
{
	InputEventType input;
	switch(n)
	{
	case 0:	input = MOUSE_BUTTON_LEFT;			break;
	case 1:	input = MOUSE_BUTTON_RIGHT;			break;
	case 2:	input = MOUSE_BUTTON_MIDDLE;		break;
	case 3:	input = MOUSE_BUTTON_WHEEL;			break;

	default:			//assume ascii value
		input = MOUSE_BUTTON_LEFT;
	}

	return input;
}