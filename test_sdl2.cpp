#include "CommonGame/Platform.h"

#ifdef PLATFORM_LINUX

#include <iostream>
#include "CommonGame/SDLGLObject.h"
#include <SDL2/SDL.h>

int main(int argc, char* argv[])
{
    std::cout << "Testing SDL2 initialization..." << std::endl;
    
    // Test SDL2 GLObject
    SDLGLObject glObject;
    
    if (!glObject.init(32, 800, 600)) {
        std::cerr << "Failed to initialize SDL2/OpenGL" << std::endl;
        return 1;
    }
    
    std::cout << "SDL2/OpenGL initialized successfully!" << std::endl;
    
    // Simple event loop
    bool running = true;
    SDL_Event e;
    
    for (int i = 0; i < 300 && running; i++) { // Run for about 5 seconds at 60 FPS
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }
        
        // Clear screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Swap buffers
        glObject.swapBuffers();
        
        SDL_Delay(16); // ~60 FPS
    }
    
    glObject.cleanup();
    std::cout << "Test completed successfully!" << std::endl;
    return 0;
}

#else
int main(int argc, char* argv[])
{
    std::cout << "This test is only for Linux platform" << std::endl;
    return 0;
}
#endif