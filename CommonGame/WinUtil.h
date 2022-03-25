#ifndef _WINUTIL_H_
#define _WINUTIL_H_

#include <string>

#include <windows.h>

using namespace::std;

string getLastWindowsError();
string getLocalTime();		//h/h:mmAM/PM
string getLocalTime24();	//hh:mm:ss

BOOL getCursorClientPos(HWND hWnd, LPPOINT p);

#endif