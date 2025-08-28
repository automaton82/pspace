#ifndef _SDL_GLOBJECT_H_
#define _SDL_GLOBJECT_H_

#include "Platform.h"
#include "DataTypes.h"
#include "Singleton.h"

class SDLGLObject
{
public:
	SDLGLObject();
	~SDLGLObject();

	bool init(int colorDepth = 32, int width = 800, int height = 600);
	bool cleanup();

	bool getRenderState(int timeout = 0);
	bool releaseRenderState();

	SDL_Window* getWindow() const { return window_; }
	SDL_GLContext getContext() const { return glContext_; }

	void swapBuffers();

private:
	SDL_Window* window_;
	SDL_GLContext glContext_;
	bool initialized_;
	SDL_mutex* renderMutex_;
};

// Global instance
extern SDLGLObject& sdlGLObject;

#endif // _SDL_GLOBJECT_H_