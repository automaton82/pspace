//David Millman

#ifndef _SUBSPACETHOR_H_
#define _SUBSPACETHOR_H_

#include "SubspaceBomb.h"
#include "TextureAnimated.h"

class SubspaceThor :
	public SubspaceBomb
{
public:
	SubspaceThor();
	~SubspaceThor();

	WeaponData getWeaponSettings() const;

	void update(double timeStep);

};

#endif