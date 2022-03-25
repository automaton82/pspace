// David Millman

#ifndef _THREAD_H_
#define _THREAD_H_

#include <windows.h>
#include <process.h>

#include "DataTypes.h"

class Thread
{
public:
	typedef void(*startRoutine)(void*);

	Thread();
	Thread(startRoutine function);
	
	bool run(startRoutine function, void* arg = 0);		//TODO: create thread function manager
	bool start(void* arg = 0);
	bool stop(Uint32 timeout = 10);
	bool waitForExit(Uint32 timeout = INFINITE);
	
	DWORD getID()	{ return threadId_; }

private:
	
	DWORD threadId_;
	HANDLE threadHandle_;
	startRoutine threadFunc_;
};

#endif