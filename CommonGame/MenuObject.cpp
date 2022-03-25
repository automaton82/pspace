#include "MenuObject.h"

MenuObject::MenuObject(double width, double height) : 
	callback_(0)
{
	resize(width, height);
}

MenuObject::~MenuObject()
{
}

void MenuObject::setCallback(MenuCallback* callback)
{
	callback_ = callback;
}

bool MenuObject::handleInput(InputEvent input, double x, double y)
{
	bool retval = false;

	if(!isInBounds(x, y))
		return retval;
		
	switch(input.type)
	{
		case MOUSE_BUTTON_LEFT:
		case MOUSE_BUTTON_RIGHT:
			if(input.isActivated())
			{
				this->onSelect();
				retval = true;
			}
			break;

		case MOUSE_PASSIVE:
			this->onHover();
			retval = true;
			break;
	}

	return retval;
}

void MenuObject::onHover()
{
	//nothing
}

void MenuObject::onSelect()
{
	if(callback_)
		callback_->call();
}

void MenuObject::draw() const
{
	//nothing
}

void MenuObject::update(double time)
{
	//nothing
}