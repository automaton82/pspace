#ifndef _WINDOWSCONSOLE_H_
#define _WINDOWSCONSOLE_H_

#include <windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <fstream.h>

int CreateConsole()
{
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
}

#endif