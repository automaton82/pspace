//David Millman

#ifndef _SUBSPACEEXHAUST_H_
#define _SUBSPACEEXHAUST_H_

#include "SubspaceSpecialEffect.h"

class SubspaceExhaust : 
	public SubspaceSpecialEffect
{
public:
	SubspaceExhaust(bool rocket = false);
	~SubspaceExhaust();

	void update(double time);

	void draw() const;

private:
	bool isRocket_;
	Vector startVelocity_;
	bool started_;
	double startTime_;
};

#endif