#ifndef _INPUTCOMMANDRECEIVER_H_
#define _INPUTCOMMANDRECEIVER_H_

#include "CommandListenerBase.h"

#include "InputEvent.h"

#include "InputCommands.h"
using InputCommands::InputData;

class InputCommandReceiver : 
	public CommandListenerBase<InputCommandReceiver, InputCommand>
{
public:

	virtual void doInputActive(const InputData& input) {}
	virtual void doInputActivated(const InputData& input) {}
	virtual void doInputUnactivated(const InputData& input) {}
};

#endif
