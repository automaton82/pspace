#include "Platform.h"

#ifdef PLATFORM_LINUX

#include <SDL2/SDL_image.h>
#include <iostream>

// Stub implementation of auxDIBImageLoad for compatibility
AUX_IMAGE* auxDIBImageLoad(const char* filename)
{
    // For now, return null to avoid crashes
    // TODO: Implement proper SDL2_image loading
    std::cerr << "Warning: auxDIBImageLoad not implemented for " << filename << std::endl;
    return nullptr;
}

#endif // PLATFORM_LINUX