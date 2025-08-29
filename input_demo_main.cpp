#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <vector>
#include <memory>
#include "SDLInputBridge.h"
#include "InputCommandGenerator.h"

#include "CommonGame/SDLGLObject.h"

class InputCommandDemo {
private:
    SDLInputBridge inputBridge;
    InputCommandGenerator commandGenerator;
    float triangleRotation = 0.0f;
    bool isLeftPressed = false;
    bool isRightPressed = false;
    bool running = true;
    
    SDLGLObject& sdlGL;

public:
    InputCommandDemo() : sdlGL(sdlGLObject) {
        std::cout << "InputCommandDemo: Initializing input command system..." << std::endl;
    }

    void handleSDLEvent(const SDL_Event& event) {
        if (event.type == SDL_QUIT) {
            running = false;
            return;
        }
        
        // Process events through the InputCommandGenerator directly using SDL event structures
        std::vector<std::unique_ptr<InputCommand>> commands;
        
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            bool isPressed = (event.type == SDL_KEYDOWN);
            commands = commandGenerator.processKeyboardEvent(event.key, isPressed);
            
            // Map SDL key to InputEventType for our demo logic
            InputEventType inputType = inputBridge.mapKey(event.key.keysym.sym);
            
            std::cout << "InputCommandDemo: Processing keyboard event type: " << inputType << std::endl;
            
            // Execute generated commands
            for (auto& command : commands) {
                if (command) {
                    std::cout << "InputCommandDemo: Executing command..." << std::endl;
                    command->execute(nullptr);
                }
            }
            
            // Apply visual feedback based on input
            if (inputType == KEY_LEFT) {
                if (isPressed) {
                    isLeftPressed = true;
                    std::cout << "InputCommandDemo: Left key activated - triangle will spin left" << std::endl;
                } else {
                    isLeftPressed = false;
                    std::cout << "InputCommandDemo: Left key deactivated" << std::endl;
                }
            } else if (inputType == KEY_RIGHT) {
                if (isPressed) {
                    isRightPressed = true;
                    std::cout << "InputCommandDemo: Right key activated - triangle will spin right" << std::endl;
                } else {
                    isRightPressed = false;
                    std::cout << "InputCommandDemo: Right key deactivated" << std::endl;
                }
            } else if (inputType == KEY_ESCAPE) {
                running = false;
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
            bool isPressed = (event.type == SDL_MOUSEBUTTONDOWN);
            commands = commandGenerator.processMouseButtonEvent(event.button, isPressed);
            
            std::cout << "InputCommandDemo: Processing mouse button event" << std::endl;
            
            // Execute generated commands
            for (auto& command : commands) {
                if (command) {
                    std::cout << "InputCommandDemo: Executing mouse command..." << std::endl;
                    command->execute(nullptr);
                }
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            commands = commandGenerator.processMouseMotionEvent(event.motion);
            
            // Execute any passive commands (usually none for motion)
            for (auto& command : commands) {
                if (command) {
                    command->execute(nullptr);
                }
            }
        }
    }

    void render() {
        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Update rotation based on input
        if (isLeftPressed) {
            triangleRotation -= 2.0f;
        }
        if (isRightPressed) {
            triangleRotation += 2.0f;
        }
        
        // Draw rotating triangle
        glLoadIdentity();
        glRotatef(triangleRotation, 0.0f, 0.0f, 1.0f);
        
        // Color changes based on input
        if (isLeftPressed && isRightPressed) {
            glColor3f(1.0f, 0.0f, 1.0f); // Magenta when both pressed
        } else if (isLeftPressed) {
            glColor3f(0.0f, 0.0f, 1.0f); // Blue when left pressed
        } else if (isRightPressed) {
            glColor3f(1.0f, 0.0f, 0.0f); // Red when right pressed
        } else {
            glColor3f(1.0f, 1.0f, 1.0f); // White when nothing pressed
        }
        
        glBegin(GL_TRIANGLES);
            glVertex2f(0.0f, 0.5f);
            glVertex2f(-0.5f, -0.5f);
            glVertex2f(0.5f, -0.5f);
        glEnd();
    }

    void update() {
        // Keep rotation in reasonable range
        if (triangleRotation > 360.0f) triangleRotation -= 360.0f;
        if (triangleRotation < -360.0f) triangleRotation += 360.0f;
    }
    
    void run() {
        // Initialize SDL and OpenGL
        if (!sdlGL.init()) {
            throw std::runtime_error("Failed to initialize SDL/OpenGL");
        }
        
        SDL_Event event;
        while (running) {
            // Handle events
            while (SDL_PollEvent(&event)) {
                handleSDLEvent(event);
            }
            
            // Update and render
            update();
            
            if (sdlGL.getRenderState()) {
                render();
                sdlGL.swapBuffers();
                sdlGL.releaseRenderState();
            }
            
            SDL_Delay(16); // ~60 FPS
        }
        
        sdlGL.cleanup();
    }
};

int main() {
    std::cout << "Starting Input Command System Demo..." << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  Left Arrow Key - Rotate triangle left (blue)" << std::endl;
    std::cout << "  Right Arrow Key - Rotate triangle right (red)" << std::endl;
    std::cout << "  Both Keys - Magenta color" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << std::endl;
    
    try {
        InputCommandDemo demo;
        demo.run();
        std::cout << "Input Command Demo completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
