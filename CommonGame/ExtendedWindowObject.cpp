#include "ExtendedWindowObject.h"

#include "GLUtil.h"

ExtendedWindowObject::ExtendedWindowObject() :
	backgroundColor_(Color(0.1, 0.1, 0.1, 0.9)),
	borderColor_(Color(1.0, 1.0, 1.0, 1.0)),
	borderWidth_(2.0)
{
}

ExtendedWindowObject::~ExtendedWindowObject()
{
}

void ExtendedWindowObject::setBackgroundColor(const Color& c)
{
	backgroundColor_ = c;
}

void ExtendedWindowObject::setBorderColor(const Color& c)
{
	borderColor_ = c;
}

void ExtendedWindowObject::setBorderWidth(double width)
{
	borderWidth_ = width;
}

void ExtendedWindowObject::draw() const
{
	glDisable(GL_TEXTURE_2D);

	backgroundColor_.enable();
	//draw background
	GLUtil::drawQuad(width_, height_);

	borderColor_.enable();
	glLineWidth((float)borderWidth_);
	//draw a rectangle
	GLUtil::drawLineRect(width_, height_);
}

void ExtendedWindowObject::drawBackground() const
{
	glDisable(GL_TEXTURE_2D);

	backgroundColor_.enable();
	//draw background
	GLUtil::drawQuad(width_, height_);

}

void ExtendedWindowObject::drawBorder() const
{
	glDisable(GL_TEXTURE_2D);

	borderColor_.enable();
	glLineWidth((float)borderWidth_);
	//draw a rectangle
	GLUtil::drawLineRect(width_, height_);
}
