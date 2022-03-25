#include "MenuManager.h"

#include <assert.h>
#include "GLUtil.h"
#include "TextureAnimated.h"

MenuManager::MenuManager()
{
}

MenuManager::~MenuManager()
{
}

bool MenuManager::handleInput(InputEvent input, double x, double y)
{
	bool handled = false;

	MenuList::iterator i;
	for(i = menuObjects_.begin(); i != menuObjects_.end(); ++i)
	{
		MenuData& md = (*i);

		if(isInMenuBounds(md, x, y))
		{
			handled = md.menu->handleInput(input, x - md.x, y - md.y);	//convert to window coordinates
			if(handled)
				break;
		}
	}

	return handled;
}

bool MenuManager::addMenuObject(MenuObject* menu, double x, double y)
{
	assert(menu);

	MenuData md;
	md.menu = menu;
	md.x = x;
	md.y = y;
	
	menuObjects_.push_back(md);

	return true;
}

bool MenuManager::isInMenuBounds(const MenuData& md, double x, double y) const
{
	bool retval = false;

	double left, right, top, bottom;
	left = md.x;
	right = left + md.menu->getWidth();
	bottom = md.y;
	top = bottom + md.menu->getHeight();

	if(x >= left && x < right &&		//check if window in range
		y >= bottom && y < top)
	{
		retval = true;
	}

	return retval;
}

void MenuManager::draw() const
{
	MenuList::const_reverse_iterator i;
	for(i = menuObjects_.rbegin(); i != menuObjects_.rend(); ++i)		//draw back to front
	{
		const MenuData& md = (*i);

		glPushMatrix();
			glTranslated(md.x, md.y, 0);
			md.menu->draw();
		glPopMatrix();
	}
}

void MenuManager::update(double time)
{
	MenuList::iterator i;
	for(i = menuObjects_.begin(); i != menuObjects_.end(); ++i)
	{
		MenuData& md = (*i);

		md.menu->update(time);
	}
}