//David Millman

#ifndef _SUBSPACELIFEBAR_H_
#define _SUBSPACELIFEBAR_H_

#include "LifeBar.h"

class SubspaceLifeBar : 
	public LifeBar
{
public:
	SubspaceLifeBar(Uint width, Uint height, double lifePercent = 100.0);
	~SubspaceLifeBar();
	
	void init();

	void setMaxLifePercent(double maxLife);	//percent of bar that the life% takes up
	double getMaxLifePercent() const;

	//display
	void draw() const;

private:
	const static double topHeight;
	const static double bottomHeight;
	const static double padding;

	double maxLifePercent_;
};

#endif