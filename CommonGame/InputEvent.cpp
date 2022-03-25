#include "InputEvent.h"

//bool InputEvent::hasStringInputs_ = false;
bool InputEvent::inputStates[INPUT_NumInputs];

InputEvent::InputEvent() : 
	type(INPUT_UNKNOWN), state(INPUT_STATE_UNKNOWN), previousState(INPUT_STATE_UNKNOWN)
{
	//initStringInputs();
}

/*InputEvent::InputEvent(const string& s) : InputEvent()
{
	type = getInputType(s);
}*/

InputEvent::InputEvent(InputEventType t, InputEventState s, InputEventState p) :
	type(t), state(s), previousState(p)
{
	//initStringInputs();
}

bool InputEvent::isAscii() const
{
	if(type > INPUT_EVENT_UNKNOWN && type < INPUT_LAST_ASCII)
		return true;
	else
		return false;
}

char InputEvent::getAscii() const
{
	return type;
}

bool InputEvent::isActivated() const
{
	if(state == INPUT_STATE_DOWN && previousState == INPUT_STATE_UP)
		return true;
	else
		return false;
}

bool InputEvent::isActive() const
{
	if(state == INPUT_STATE_DOWN)
		return true;
	else
		return false;
}

bool InputEvent::isUnactivated() const
{
	if(state == INPUT_STATE_UP && previousState == INPUT_STATE_DOWN)
		return true;
	else
		return false;
}

/*void InputEvent::initStringInputs()
{
	if(hasStringInputs_)
		return;

	stringInputs_["UP"]		= KEY_UP; 
	stringInputs_["DOWN"]	= KEY_DOWN; 
	stringInputs_["LEFT"]	= KEY_LEFT; 
	stringInputs_["RIGHT"]	= KEY_RIGHT;

	stringInputs_["LCONTROL"]	= KEY_LCONTROL;
	stringInputs_["RCONTROL"]	= KEY_RCONTROL;
	stringInputs_["LSHIFT"]		= KEY_LSHIFT;
	stringInputs_["RSHIFT"]		= KEY_RSHIFT;

	stringInputs_["TAB"]	= KEY_TAB;

	stringInputs_["INSERT"]	= KEY_INSERT;
	stringInputs_["DELETE"]	= KEY_DELETE;
	stringInputs_["HOME"]	= KEY_HOME;
	stringInputs_["END"]	= KEY_END;
	stringInputs_["PAGEUP"]	= KEY_PAGEUP;
	stringInputs_["PAGEDOWN"] = KEY_PAGEDOWN;

	stringInputs_["ESCAPE"] = KEY_ESCAPE;
						
	stringInputs_["F1"] = KEY_F1;
	stringInputs_["F2"] = KEY_F2;
	stringInputs_["F3"] = KEY_F3;
	stringInputs_["F4"] = KEY_F4;
	stringInputs_["F5"] = KEY_F5;
	stringInputs_["F6"] = KEY_F6;
	stringInputs_["F7"] = KEY_F7;
	stringInputs_["F8"] = KEY_F8;
	stringInputs_["F9"]	= KEY_F9;
	stringInputs_["F10"] = KEY_F10;
	stringInputs_["F11"] = KEY_F11;
	stringInputs_["F12"] = KEY_F12;

	stringInputs_["MOUSE_LEFT"] = MOUSE_LEFT;
	stringInputs_["MOUSE_RIGHT"] = MOUSE_RIGHT;
	stringInputs_["MOUSE_MIDDLE"] = MOUSE_MIDDLE;

	hasStringInputs_ = true;
}

InputEventType InputEvent::getInputType(const string& s)
{
	if(s.size() == 1)		// ascii char
	{
		return (InputEventType)s[0];
	}

	string name = AsciiUtil::toupper(s);
	
	if(stringInputs_.find(name))
		return stringInputs_[name];
	else
		return INPUT_UNKNOWN;
}*/