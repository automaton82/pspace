// David Millman

#include "Thread.h"

#include <iostream>
#include "WinUtil.h"

Thread::Thread() : 
	threadId_(0), threadHandle_(0), 
	threadFunc_(0)
{
}

Thread::Thread(Thread::startRoutine function) : 
	threadId_(0), threadHandle_(0), 
	threadFunc_(function)
{
}

bool Thread::run(startRoutine function, void* arg)
{
	stop();

	threadFunc_ = function;
	
	return start(arg);
}

bool Thread::start(void* arg)
{
	stop();

	threadHandle_ = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadFunc_, arg, 0, &threadId_);

	//uintptr_t retval = _beginthread(static_cast<Thread::startRoutine>(threadFunc_), 0, arg);
	//threadHandle_ = (HANDLE)retval;

	if(threadHandle_ != 0) 
		return true;
	else
		return false;
}

bool Thread::stop(Uint32 timeout)
{
	bool stopped = false;

	if(threadHandle_)
	{
		printf("Stopping thread \"%d\"...\n", threadId_);

		bool closed = (TerminateThread(threadHandle_, 0) == TRUE);		
		if(!closed)
		{
			printf("Error- Thread.stop(), TerminateThread(): %s\n", getLastWindowsError().c_str());
		}

		DWORD retval = WaitForSingleObject(threadHandle_, timeout);
		if(retval == WAIT_FAILED)
		{
			printf("Error- Thread.stop(), WaitForSingleObject: %s\n", getLastWindowsError().c_str());
		}
		else
		{
			printf("Thread \"%d\" stopped.\n", threadId_);
		
			threadHandle_ = 0;
			stopped = true;
		}
	}
	
	return stopped;
}

bool Thread::waitForExit(Uint32 timeout)
{
	bool stopped = false;

	if(threadHandle_)
	{
		printf("Waiting for thread \"%d\"...\n", threadId_);

		DWORD retval = WaitForSingleObject(threadHandle_, timeout);
		if(retval == WAIT_FAILED)
		{
			printf("Error- Thread.waitForExit(): %s\n", getLastWindowsError().c_str());
		}
		else
		{
			printf("Thread \"%d\" stopped.\n", threadId_);
		
			threadHandle_ = 0;
			stopped = true;
		}
	}
	
	return stopped;
}


