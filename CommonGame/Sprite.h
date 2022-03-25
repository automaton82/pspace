//David Millman

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "GameObject.h"
#include "TextureAnimated.h"
#include "Texturizable.h"

class Sprite : 
	public GameObject,
	public Texturizable
{
public:
	Sprite();
	~Sprite();

	double getDisplayWidth() const;
	double getDisplayHeight() const;

	void setTexture(const TextureAnimated& texture);
	const TextureAnimated* getTexture() const;
	TextureAnimated* getTexture();

	void update(double timeMultiplier);

    //does only drawing associated with the actual object (i.e. no glTranslate for position)
	void draw() const;

private:

	TextureAnimated currentTexture_;
};

#endif