#ifndef _COMMANDLISTENERBASE_H_
#define _COMMANDLISTENERBASE_H_

#include <assert.h>

#include <list>
using std::list;

#include "CommandGenerator.h"

// TODO: move this into each corresponding listener type?
template <class ListenerType, class CommandType>
class CommandListenerBase
{
public:
	virtual void handleCommand(CommandType* command, CommandGenerator<ListenerType>* from = 0)
	{
		ListenerType* me = dynamic_cast<ListenerType*>(this);
		assert(me);
		command->execute(me);
	}
};

#endif