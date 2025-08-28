// David Millman

#include "Timer.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <SDL2/SDL.h>
#include <sys/time.h>
#endif

Timer::Timer()
{
#ifdef _WIN32
	QueryPerformanceFrequency(&ticksPerSecond_);
	QueryPerformanceCounter(&startTick_);
	stopTick_.QuadPart = startTick_.QuadPart;
#else
	// Use SDL ticks for Linux
	ticksPerSecond_ = 1000; // SDL_GetTicks() returns milliseconds
	startTick_ = SDL_GetTicks();
	stopTick_ = startTick_;
#endif
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
#ifdef _WIN32
		startTick_.u = rhs.startTick_.u;
		stopTick_.u = rhs.stopTick_.u;
		ticksPerSecond_.u = rhs.ticksPerSecond_.u;
#else
		startTick_ = rhs.startTick_;
		stopTick_ = rhs.stopTick_;
		ticksPerSecond_ = rhs.ticksPerSecond_;
#endif

		paused = rhs.paused;
	}
	return *this;
}

void Timer::start()
{
#ifdef _WIN32
	QueryPerformanceCounter(&startTick_);
	stopTick_.QuadPart = 0;
#else
	startTick_ = SDL_GetTicks();
	stopTick_ = 0;
#endif
	paused = false;
}

void Timer::unpause()
{
	if(!paused)
		return;

#ifdef _WIN32
	LARGE_INTEGER currentTick;
	QueryPerformanceCounter(&currentTick);

	startTick_.QuadPart = currentTick.QuadPart - (stopTick_.QuadPart - startTick_.QuadPart);
	stopTick_.QuadPart = currentTick.QuadPart;	
#else
	long long currentTick = SDL_GetTicks();
	startTick_ = currentTick - (stopTick_ - startTick_);
	stopTick_ = currentTick;
#endif
	paused = false;	
}

void Timer::pause()
{
	if(!paused)
	{
#ifdef _WIN32
		QueryPerformanceCounter(&stopTick_);
#else
		stopTick_ = SDL_GetTicks();
#endif
		paused = true;
	}
}

bool Timer::isPaused()
{
	return paused;
}

void Timer::stop()
{
#ifdef _WIN32
	QueryPerformanceCounter(&startTick_);
	stopTick_ = startTick_;	
#else
	startTick_ = SDL_GetTicks();
	stopTick_ = startTick_;
#endif
}

double Timer::getElapsedTime() const
{
#ifdef _WIN32
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
#else
	long long currentTick;
	if(paused)
	{
		currentTick = stopTick_;
	}
	else
	{
		currentTick = SDL_GetTicks();
	}

	return ((currentTick - startTick_) * 1000.0 / (double)ticksPerSecond_);
#endif
}