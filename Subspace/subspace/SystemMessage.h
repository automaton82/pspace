#ifndef _SYSTEMMESSAGE_H_
#define _SYSTEMMESSAGE_H_

struct SystemMessage
{
	SystemMessage();
	SystemMessage(string text, int fontSet);

	string text;
	int color;		//corresponds to a font set color in the bmp
};

#endif