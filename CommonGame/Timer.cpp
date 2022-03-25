// David Millman

#include "Timer.h"

Timer::Timer()
{
	QueryPerformanceFrequency(&ticksPerSecond_);
	QueryPerformanceCounter(&startTick_);
	stopTick_.QuadPart = startTick_.QuadPart;
	paused = true;
}

Timer::Timer(const Timer& copy)
{
	*this = copy;
}

Timer& Timer::operator =(const Timer& rhs)
{
	if(this != &rhs)
	{
		startTick_.u = rhs.startTick_.u;
		stopTick_.u = rhs.stopTick_.u;
		ticksPerSecond_.u = rhs.ticksPerSecond_.u;

		paused = rhs.paused;
	}
	return *this;
}

void Timer::start()
{
	QueryPerformanceCounter(&startTick_);
	stopTick_.QuadPart = 0;
	paused = false;
}

void Timer::unpause()
{
	if(!paused)
		return;

	LARGE_INTEGER currentTick;
	QueryPerformanceCounter(&currentTick);

	startTick_.QuadPart = currentTick.QuadPart - (stopTick_.QuadPart - startTick_.QuadPart);
	stopTick_.QuadPart = currentTick.QuadPart;	
	paused = false;	
}

void Timer::pause()
{
	if(!paused)
	{
		QueryPerformanceCounter(&stopTick_);
		paused = true;
	}
}

bool Timer::isPaused()
{
	return paused;
}

void Timer::stop()
{
	QueryPerformanceCounter(&startTick_);
	stopTick_ = startTick_;	
}

double Timer::getElapsedTime() const
{
	LARGE_INTEGER currentTick;
	if(paused)
	{
		currentTick.u = stopTick_.u;
	}
	else
	{
		QueryPerformanceCounter(&currentTick);
	}

	return ((currentTick.QuadPart - startTick_.QuadPart)*1000.0 / (double)ticksPerSecond_.QuadPart);
}