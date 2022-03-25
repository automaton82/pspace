//David Millman

#ifndef _SUBSPACEDECOY_H_
#define _SUBSPACEDECOY_H_

class SubspacePlayer;

#include "SubspaceWeapon.h"
#include "TextureAnimated.h"

class SubspaceDecoy :
	public SubspaceWeapon
{
public:
	SubspaceDecoy(SubspacePlayer* player);
	~SubspaceDecoy();
	
	void setPlayer(SubspacePlayer* player);

	WeaponData getWeaponSettings() const;

	bool isBouncy() const;
	void draw() const;

	void update(double timeStep);

private:

	SubspacePlayer* player_;
	TextureAnimated decoyTexture_;

	int oppositeDir_, lastDir_;
};

#endif