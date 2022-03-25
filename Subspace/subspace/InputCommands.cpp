#include "InputCommands.h"

#include "InputCommandReceiver.h"

InputCommand::~InputCommand()
{
}

namespace InputCommands
{
	
void InputActive::execute(InputCommandReceiver* receiver)
{
	receiver->doInputActive(input);
}

void InputActivated::execute(InputCommandReceiver* receiver)
{
	receiver->doInputActivated(input);
}

void InputUnactivated::execute(InputCommandReceiver* receiver)
{
	receiver->doInputUnactivated(input);
}

}