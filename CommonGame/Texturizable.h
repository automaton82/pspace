#ifndef _TEXTURIZABLE_H_
#define _TEXTURIZABLE_H_

#include "Texture.h"

class Texturizable
{
public:

	Texturizable();
	virtual ~Texturizable();

	virtual const Texture* getTexture() const;
	virtual Texture* getTexture();
	virtual void setTexture(Texture* texture);

private:
	Texture* texture;
};

#endif