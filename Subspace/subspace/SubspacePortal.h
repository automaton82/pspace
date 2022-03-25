//David Millman

#ifndef _SUBSPACEPORTAL_H_
#define _SUBSPACEPORTAL_H_

#include "SubspaceWeapon.h"
#include "TextureAnimated.h"

class SubspacePortal :
	public SubspaceWeapon
{
public:
	SubspacePortal();
	~SubspacePortal();

	void update(double timeMultiplier);

	void draw() const;

private:
	
	TextureAnimated portalTexture_;
};

#endif