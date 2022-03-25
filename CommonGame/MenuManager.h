#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

#include <list>
using std::list;

#include "InputEvent.h"
#include "MenuObject.h"
#include "OrthographicObject.h"
#include "WindowObject.h"

class MenuManager : 
	public OrthographicObject
{
private:

	struct MenuData
	{
		MenuData() : menu(0), x(0), y(0)
		{}

		MenuObject* menu;
		double x, y;
	};

	typedef list<MenuData> MenuList;

public:
	MenuManager();
	virtual ~MenuManager();
	
	bool handleInput(InputEvent input, double x, double y);		//pass input to appropriate menu object

	bool addMenuObject(MenuObject* menu, double x=0, double y=0);

	void draw() const;	//takes care of window ordering
	void update(double time);

private:

	bool isInMenuBounds(const MenuData& menuData, double x, double y) const;

private:

	MenuList menuObjects_;
	WindowObject* selectedMenuObject_;
};

#endif