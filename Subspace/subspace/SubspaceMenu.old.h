#ifndef _SUBSPACEMENU_H_
#define _SUBSPACEMENU_H_

#include "TextTable.h"

class SubspaceMenu : 
	public TextTable
{
public:

	const static Uint menuWidth = 35;
	const static Uint menuHeight = 14;

public:
	SubspaceMenu();

	void init();
		
public:
	
	TextureFont font_;
};

#endif