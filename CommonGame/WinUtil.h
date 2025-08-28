#ifndef _WINUTIL_H_
#define _WINUTIL_H_

#include <string>

#ifdef _WIN32
#include <windows.h>
#else
// Linux compatibility types
typedef int BOOL;
typedef void* HWND;
typedef struct { int x, y; } POINT;
typedef POINT* LPPOINT;
#define TRUE 1
#define FALSE 0
#endif

using namespace::std;

string getLastWindowsError();
string getLocalTime();		//h/h:mmAM/PM
string getLocalTime24();	//hh:mm:ss

BOOL getCursorClientPos(HWND hWnd, LPPOINT p);

#endif