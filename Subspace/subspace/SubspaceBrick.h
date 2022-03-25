//David Millman

#ifndef _SUBSPACEBRICK_H_
#define _SUBSPACEBRICK_H_

#include "SubspaceWeapon.h"
#include "TextureAnimated.h"

class SubspaceBrick :
	public SubspaceWeapon			//TODO: not exactly a weapon, but close enough?
{
public:
	SubspaceBrick(Uint team, Uint myTeam);
	~SubspaceBrick();

	Uint getTeam() const;

	void setTeam(Uint team);
	void setMyTeam(Uint myTeam);

	void setTexture();

	void update(double time);

	void draw() const;

private:

	Uint team_, myTeam_;
	TextureAnimated brickTexture_;
};

#endif