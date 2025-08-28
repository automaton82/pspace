// David Millman

#ifndef _TIMER_H_
#define _TIMER_H_

#include "Platform.h"
#include <stdio.h>

/**
* Defines a Timer class based off of the performance counter
*
* @author	Dave Millman (millmd@rpi.edu)
*/

class Timer
{
public:
	Timer();
	Timer(const Timer& copy);
	Timer& operator =(const Timer& rhs);

	///starts the timer going
	void start();

	///continues the timer after being stopped
	void unpause();

	///pauses the counter
	void pause();

	//stops and resets the timer
	void stop();

	bool isPaused();
	
	///total non-stopped time, in milliseconds
	double getElapsedTime() const;
private:
	
#ifdef _WIN32
	LARGE_INTEGER ticksPerSecond_;
	LARGE_INTEGER startTick_;
	LARGE_INTEGER stopTick_;
#else
	// Linux compatibility - use timespec or similar
	long long ticksPerSecond_;
	long long startTick_;
	long long stopTick_;
#endif

	bool paused;
};

#endif