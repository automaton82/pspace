#ifndef _INPUTCOMMANDLISTENER_H_
#define _INPUTCOMMANDLISTENER_H_

#include "InputEvent.h"

#include "InputCommands.h"

class InputCommandListener : 
	public CommandListenerBase<InputCommandListener, InputCommand>
{
public:

	virtual void doInputActive(InputEventType inputType);
	virtual void doInputActivated(InputEventType inputType);
	virtual void doInputUnactivated(InputEventType inputType);
};

#endif
