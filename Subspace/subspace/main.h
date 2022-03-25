#ifndef _MAIN_H_
#define _MAIN_H_

#define WIN32_LEAN_AND_MEAN
#define STRICT

#include <windows.h>		// Header File For Windows

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

//Window
LRESULT CALLBACK WndProc(HWND, UINT,WPARAM, LPARAM);

//static HWND g_hWnd=NULL;		// Holds Our Window Handle
//static HINSTANCE g_hInstance;		// Holds The Instance Of The Application

//Display
bool CreateGLWindow(char* title, int width, int height, int bits);
void DrawGLScene();
void KillGLWindow();							// Properly Kill The Window
void ReSizeGLScene(int width, int height);		// Resize And Initialize The GL Window
bool InitGL();

#include "InputEvent.h"

//Game 
void GameInit();
void GameDestroy();
void GameInput(InputEvent input);
void GameLoop();
void GameTick(double value);
void GameResize(int width, int height);
void GameDisplay();


#endif