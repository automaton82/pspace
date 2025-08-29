#include "Platform.h"

#ifdef PLATFORM_LINUX

#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

// SDL2-based implementation of auxDIBImageLoad for .bm2 files
AUX_RGBImageRec* auxDIBImageLoad(const char* filename)
{
	// Load image using SDL2_image
	SDL_Surface* surface = IMG_Load(filename);
	if (!surface) {
		printf("Warning: Failed to load image %s: %s\n", filename, IMG_GetError());
		return NULL;
	}
	
	// Convert to RGB format if necessary
	SDL_Surface* rgbSurface = NULL;
	if (surface->format->format != SDL_PIXELFORMAT_RGB24) {
		rgbSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB24, 0);
		SDL_FreeSurface(surface);
		if (!rgbSurface) {
			printf("Warning: Failed to convert image %s to RGB\n", filename);
			return NULL;
		}
		surface = rgbSurface;
	}
	
	// Create AUX_RGBImageRec structure
	AUX_RGBImageRec* imageRec = (AUX_RGBImageRec*)malloc(sizeof(AUX_RGBImageRec));
	if (!imageRec) {
		SDL_FreeSurface(surface);
		printf("Warning: Failed to allocate memory for image %s\n", filename);
		return NULL;
	}
	
	// Fill in the structure
	imageRec->sizeX = surface->w;
	imageRec->sizeY = surface->h;
	
	// Allocate and copy pixel data
	int dataSize = surface->w * surface->h * 3; // RGB = 3 bytes per pixel
	imageRec->data = (unsigned char*)malloc(dataSize);
	if (!imageRec->data) {
		free(imageRec);
		SDL_FreeSurface(surface);
		printf("Warning: Failed to allocate pixel data for image %s\n", filename);
		return NULL;
	}
	
	// Copy pixel data
	memcpy(imageRec->data, surface->pixels, dataSize);
	
	SDL_FreeSurface(surface);
	printf("Successfully loaded image: %s (%dx%d)\n", filename, imageRec->sizeX, imageRec->sizeY);
	return imageRec;
}

#endif // PLATFORM_LINUX