#ifndef _COMMANDGENERATOR_H_
#define _COMMANDGENERATOR_H_

#include <assert.h>

#include <algorithm>

#include <list>
using std::list;

// takes a type of receiver to "connect" to
template <class ListenerType>
class CommandGenerator
{
public:
	typedef list<ListenerType*> ListenerList;

	void addListener(ListenerType* listener)
	{
		if(std::find(listeners_.begin(), listeners_.end(), listener) == listeners_.end())	// check for duplicate
			listeners_.push_back(listener);
	}

	void removeListener(ListenerType* listener)
	{
		listeners_.remove(listener);
	}

	template <class CommandType> void invokeCommand(CommandType* command)
	{
		ListenerList::iterator i;
		for(i = listeners_.begin(); i != listeners_.end(); ++i)
		{
			(*i)->handleCommand(command, this);
		}
	}

protected:
	ListenerList listeners_;
};


#endif