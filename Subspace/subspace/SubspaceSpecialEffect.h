//David Millman

#ifndef _SUBSPACESPECIALEFFECT_H_
#define _SUBSPACESPECIALEFFECT_H_

#include "SubspaceGameObject.h"
#include "TextureAnimated.h"

class SubspaceSpecialEffect :
	public SubspaceGameObject
{
public:
	SubspaceSpecialEffect();
	~SubspaceSpecialEffect();

	void update(double timeStep);

	void draw() const;

protected:

	double timeToLive_;

	TextureAnimated effectTexture_;
};

#endif