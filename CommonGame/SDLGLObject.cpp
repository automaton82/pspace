#include "SDLGLObject.h"
#include <iostream>

SDLGLObject::SDLGLObject() :
	window_(nullptr),
	glContext_(nullptr),
	initialized_(false),
	renderMutex_(nullptr)
{
}

SDLGLObject::~SDLGLObject()
{
	cleanup();
}

bool SDLGLObject::init(int colorDepth, int width, int height)
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	if (colorDepth == 32) {
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	} else if (colorDepth == 16) {
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	}

	// Create window
	window_ = SDL_CreateWindow("Subspace 0.190",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (window_ == nullptr) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Create OpenGL context
	glContext_ = SDL_GL_CreateContext(window_);
	if (glContext_ == nullptr) {
		std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Enable VSync
	if (SDL_GL_SetSwapInterval(1) < 0) {
		std::cerr << "Warning: Unable to set VSync! SDL_Error: " << SDL_GetError() << std::endl;
	}

	// Create mutex for thread safety
	renderMutex_ = SDL_CreateMutex();
	if (renderMutex_ == nullptr) {
		std::cerr << "Could not create mutex! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	initialized_ = true;
	return true;
}

bool SDLGLObject::cleanup()
{
	if (renderMutex_) {
		SDL_DestroyMutex(renderMutex_);
		renderMutex_ = nullptr;
	}

	if (glContext_) {
		SDL_GL_DeleteContext(glContext_);
		glContext_ = nullptr;
	}

	if (window_) {
		SDL_DestroyWindow(window_);
		window_ = nullptr;
	}

	SDL_Quit();
	initialized_ = false;
	return true;
}

bool SDLGLObject::getRenderState(int timeout)
{
	if (!initialized_ || !renderMutex_) {
		return false;
	}

	int result;
	if (timeout == 0) {
		result = SDL_LockMutex(renderMutex_);
	} else {
		// SDL doesn't have timeout lock, so we use try lock in a loop
		result = SDL_TryLockMutex(renderMutex_);
		if (result == SDL_MUTEX_TIMEDOUT) {
			return false;
		}
	}

	if (result == 0) {
		// Make the context current
		if (SDL_GL_MakeCurrent(window_, glContext_) != 0) {
			SDL_UnlockMutex(renderMutex_);
			return false;
		}
		return true;
	}

	return false;
}

bool SDLGLObject::releaseRenderState()
{
	if (!initialized_ || !renderMutex_) {
		return false;
	}

	SDL_UnlockMutex(renderMutex_);
	return true;
}

void SDLGLObject::swapBuffers()
{
	if (window_) {
		SDL_GL_SwapWindow(window_);
	}
}

// Global instance using Singleton pattern
SDLGLObject& sdlGLObject = *Singleton<SDLGLObject>::instance();