#ifndef _SDL_MAIN_H_
#define _SDL_MAIN_H_

#include "Platform.h"
#include "InputEvent.h"

#ifdef PLATFORM_LINUX

// SDL2-based function declarations
bool CreateSDLWindow(const char* title, int width, int height, int bits);
void DrawGLScene();
void KillSDLWindow();
void ReSizeGLScene(int width, int height);
bool InitGL();

// Game functions
void GameInit();
void GameDestroy();
void GameInput(InputEvent input);
void GameLoop();
void GameTick(double value);
void GameResize(int width, int height);

// SDL2 specific functions
bool HandleSDLEvents();
void SDLMessageBox(const char* message, const char* title);

#endif // PLATFORM_LINUX

#endif // _SDL_MAIN_H_