#include "LifeBar.h"

#include "GLUtil.h"
#include "Material.h"

LifeBar::LifeBar(Uint width, Uint height, double lifePercent) : 
	width_(width), height_(height), 
	lifePercent_(lifePercent), 
	texture_(0), color_(1,1,1)
{
}

LifeBar::~LifeBar()
{
	clearTexture();
}

double LifeBar::getWidth() const
{
	return width_;
}

double LifeBar::getHeight() const
{
	return height_;
}

double LifeBar::getLifePercent() const
{
	return lifePercent_;
}

void LifeBar::setWidth(double width)
{
	width_ = width;
}

void LifeBar::setHeight(double height)
{
	height_ = height;
}

void LifeBar::setColor(const Color& c)
{
	color_ = c;
}

void LifeBar::setTexture(const TextureAnimated& t)
{
	clearTexture();

	texture_ = new TextureAnimated();
	*texture_ = t;
}

void LifeBar::setLifePercent(double life)
{
	lifePercent_ = life;
}

void LifeBar::draw() const
{
	//scale texture to correct size
	Material m(color_);
	m.enable();

	glScaled(width_ / (double)texture_->getFrameWidth(), height_ / (double)texture_->getFrameHeight(), 1);

	if(texture_)
		texture_->display();	//draws a frame
	else
		GLUtil::drawUnitQuad();
}

void LifeBar::update(double time)
{
	if(texture_)
		texture_->advanceFrame(time);
}

void LifeBar::clearTexture()
{
	if(texture_)
		delete texture_;
}