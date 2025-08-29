#ifndef _WINDOWSCONSOLE_H_
#define _WINDOWSCONSOLE_H_

#ifdef _WIN32
#include <windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <fstream.h>
#else
// Linux compatibility - stub console functions
typedef void* HWND;
typedef void* HINSTANCE;
typedef unsigned int DWORD;  // Use consistent 32-bit definition
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#endif

int CreateConsole()
{
#ifdef _WIN32
	if(!AllocConsole())
	{
		return 0;
	}

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	int hCrt = _open_osfhandle((intptr_t)hStdout, _O_TEXT);	// return a runtime file handle
	FILE* hf = _fdopen( hCrt, "w" );						// stream
	setvbuf(hf, NULL, _IONBF, 1 );
	*stdout = *hf;
	ios::sync_with_stdio();

	return 1;
#else
	// Linux - console already available, just sync streams
	std::ios::sync_with_stdio();
	return 1;
#endif
}

#endif