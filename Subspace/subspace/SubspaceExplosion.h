//David Millman

#ifndef _SUBSPACEEXPLOSION_H_
#define _SUBSPACEEXPLOSION_H_

#include "SubspaceSpecialEffect.h"

class SubspaceExplosion :
	public SubspaceSpecialEffect
{
public:
	SubspaceExplosion();
	~SubspaceExplosion();

	void draw() const;
	
};

#endif