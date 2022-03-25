//David Millman

#ifndef _REPEATINGBACKGROUND_H_
#define _REPEATINGBACKGROUND_H_

#include "Background.h"

class RepeatingBackground :
	public Background
{
public:

	RepeatingBackground();
	~RepeatingBackground();

	void setRepeatSize(double x, double y);	//x, y is the size of the repeated tile

	void draw() const;
	//void display() const;

protected:

	double repeatX_, repeatY_;
};

#endif