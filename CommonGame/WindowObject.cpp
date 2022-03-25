#include "WindowObject.h"

#include "GLUtil.h"

WindowObject::WindowObject()
{
}

WindowObject::~WindowObject()
{
}

double WindowObject::getHeight() const
{
	return height_;
}

double WindowObject::getWidth() const
{
	return width_;
}

/*double WindowObject::getX() const
{
	return x_;
}

double WindowObject::getY() const
{
	return y_;
}*/

bool WindowObject::handleDragInput(InputEvent input, double x0, double y0, double x1, double y1)
{
	bool retval = false;

	return retval;
}

bool WindowObject::handleInput(InputEvent input, double x, double y)
{
	bool retval = true;

	return retval;
}

/*bool WindowObject::move(double x, double y)
{
	x_ = x;
	y_ = y;

	return true;
}*/

void WindowObject::onResize()
{
	//nothing here
}

bool WindowObject::resize(double width, double height)
{
	width_ = width;
	height_ = height;

	onResize();

	return true;
}

void WindowObject::onSelect()
{
	//nothing here
}

/*void WindowObject::display() const
{
	glPushMatrix();
		glTranslated(x_, y_, 0);
		this->draw();
	glPopMatrix();
}*/

void WindowObject::draw() const
{
	glColor4d(0.1, 0.1, 0.1, 0.9);
	//draw background
	GLUtil::drawQuad(width_, height_);

	glColor4d(1.0, 1.0, 1.0, 1.0);
	glLineWidth(2);
	//draw a rectangle
	GLUtil::drawLineRect(width_, height_);
}

void WindowObject::update(double time)
{
	//nothing to update
}

bool WindowObject::isInBounds(double x, double y) const
{
	bool retval = false;

	if(x >= 0 && x < width_ &&
		y >= 0 && y < height_)
	{
		retval = true;
	}

	return retval;
}
