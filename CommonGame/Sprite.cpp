//David Millman

#include "Sprite.h"

#include "TextureManager.h"

Sprite::Sprite()
{
	//int id = textureManager.loadTexture<TextureAnimated>("graphics/tiles.bmp");

	/*currentTexture_.load("graphics/tiles.bmp");
	
	currentTexture_.setFrameRange(40*1, 40*1 + 40);
	currentTexture_.setFrameSize(36, 36);
	//currentTexture_.setAnimationPeriod(40 * 250);
	currentTexture_.setAnimationPeriod(1.0);*/
}

Sprite::~Sprite()
{
}

double Sprite::getDisplayWidth() const
{
	return currentTexture_.getFrameWidth();
}

double Sprite::getDisplayHeight() const
{
	return currentTexture_.getFrameHeight();
}

void Sprite::setTexture(const TextureAnimated& texture)
{
	currentTexture_ = texture;
}

const TextureAnimated* Sprite::getTexture() const
{
	return &currentTexture_;
}

TextureAnimated* Sprite::getTexture()
{
	return &currentTexture_;
}

void Sprite::update(double t)
{
	GameObject::update(t);

	currentTexture_.advanceFramePercent(t);
}

void Sprite::draw() const
{
	//nothing for now
	//static_cast<const TextureAnimated*>(getTexture())->display();
	glPushMatrix();

	//center the sprite
	//glTranslatef(-18, -18, 0);
	//double moveX = -currentTexture_.getFrameWidth()/2.0;
	//double moveY = -currentTexture_.getFrameHeight()/2.0;
	glTranslated(-(double)currentTexture_.getFrameWidth()/2.0, -(double)currentTexture_.getFrameHeight()/2.0, 0);

	//double angle = atan2f(this->getDirection().y_, this->getDirection().x_)*180.0/PI;	//direction should be a unit vector
    //glRotatef(angle, 0, 0, 1);

	currentTexture_.display();

	glPopMatrix();
}