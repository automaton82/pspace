#ifndef _WINDOWGLOBAL_H_
#define _WINDOWGLOBAL_H_

#include "DataTypes.h"
#include "Singleton.h"

struct WindowGlobal
{
	Uint windowWidth;
	Uint windowHeight;
};

static WindowGlobal& windowGlobal = *Singleton<WindowGlobal>::instance();

#endif
