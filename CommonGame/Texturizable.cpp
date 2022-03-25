#include "Texturizable.h"

Texturizable::Texturizable() :
	texture(0)
{
}

Texturizable::~Texturizable()
{
}

const Texture* Texturizable::getTexture() const
{
	return texture;
}

Texture* Texturizable::getTexture()
{
	return texture;
}

void Texturizable::setTexture(Texture* t)
{
    texture = t;
}