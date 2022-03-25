#ifndef _COMMANDROUTER_H_
#define _COMMANDROUTER_H_

#include <list>
using std::list;

#include "CommandGenerator.h"

template <class ListenerType, class CommandType>
class CommandRouter : 
	public ListenerType,
	public CommandGenerator<ListenerType>
{
public:
	
	void handleCommand(CommandType* command, CommandGenerator<ListenerType>* from = 0)
	{
		cout << "Routing command..." << endl;
		ListenerList::iterator i;
		for(i = listeners_.begin(); i != listeners_.end(); ++i)
		{
			if((*i) != from)
				(*i)->handleCommand(command, from);
		}
	}
};

#endif