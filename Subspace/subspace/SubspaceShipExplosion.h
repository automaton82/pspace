//David Millman

#ifndef _SUBSPACESHIPEXPLOSION_H_
#define _SUBSPACESHIPEXPLOSION_H_

#include "SubspaceSpecialEffect.h"

class SubspaceShipExplosion :
	public SubspaceSpecialEffect
{
public:
	SubspaceShipExplosion();
	~SubspaceShipExplosion();

	void draw() const;
	
};

#endif