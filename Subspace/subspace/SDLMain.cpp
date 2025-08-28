#include "Platform.h"

#ifdef PLATFORM_LINUX

#include "SDLMain.h"
#include "main.h"

#include <math.h>
#include <time.h>
#include <iostream>

#include "GLObject.h"
#include "MathUtil.h"
#include "SoundManager.h"
#include "SubspaceGlobal.h"
#include "SubspaceSound.h"
#include "TimeProfiler.h"
#include "TimeStream.h"

static bool active = true;
static bool done = false;

int main(int argc, char* argv[])
{
	srand(time(NULL));

	subspaceGlobal.setWindowWidth(800);
	subspaceGlobal.setWindowHeight(600);
	
	// Create Our SDL Window
	if (!CreateSDLWindow("Subspace 0.190", subspaceGlobal.getWindowWidth(), subspaceGlobal.getWindowHeight(), 16))
	{
		return 1;
	}

	GameInit();

	// Main game loop
	while (!done)
	{
		if (!HandleSDLEvents()) {
			break;
		}

		if (active)
		{
			GameLoop();
		}
		else
		{
			SDL_Delay(10); // Sleep for 10ms when inactive
		}
	}

	GameDestroy();
	KillSDLWindow();
	return 0;
}

bool CreateSDLWindow(const char* title, int width, int height, int bits)
{
	if (!glObject.init(32, width, height))
	{
		SDLMessageBox("Can't Initialize SDL/OpenGL.", "ERROR");
		return false;
	}

	ReSizeGLScene(width, height);

	if (!InitGL())
	{
		KillSDLWindow();
		SDLMessageBox("Initialization Failed.", "ERROR");
		return false;
	}
	
	return true;
}

void KillSDLWindow()
{
	glObject.cleanup();
}

void ReSizeGLScene(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double ratio = (double)width / (double)height;
	double fov = atan(height / 600.0) * 180.0/MathUtil::PI;
	gluPerspective(fov, ratio, 10.0, 20000.0);

	glDepthRange(10, 20000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GameResize(width, height);
}

bool InitGL()
{
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glDisable(GL_LIGHTING);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_TEXTURE_2D);

	return true;
}

bool HandleSDLEvents()
{
	SDL_Event e;
	
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			done = true;
			return false;

		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				ReSizeGLScene(e.window.data1, e.window.data2);
			}
			else if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
			{
				active = true;
			}
			else if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
			{
				active = false;
			}
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			InputEvent input;
			input.type_ = (e.type == SDL_KEYDOWN) ? InputEvent::KEY_DOWN : InputEvent::KEY_UP;
			input.key_ = e.key.keysym.sym;
			input.x_ = 0;
			input.y_ = 0;
			GameInput(input);
			break;
		}

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		{
			InputEvent input;
			input.type_ = (e.type == SDL_MOUSEBUTTONDOWN) ? InputEvent::MOUSE_DOWN : InputEvent::MOUSE_UP;
			input.key_ = e.button.button;
			input.x_ = e.button.x;
			input.y_ = e.button.y;
			GameInput(input);
			break;
		}

		case SDL_MOUSEMOTION:
		{
			InputEvent input;
			input.type_ = InputEvent::MOUSE_MOVE;
			input.key_ = 0;
			input.x_ = e.motion.x;
			input.y_ = e.motion.y;
			GameInput(input);
			break;
		}
		}
	}
	
	return true;
}

void SDLMessageBox(const char* message, const char* title)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, nullptr);
}

#endif // PLATFORM_LINUX