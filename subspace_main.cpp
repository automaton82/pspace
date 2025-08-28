#include "SDLGLObject.h"
#include "SoundManager.h"
#include "SubspaceMock.h"  // Use mock instead of full SubspaceGlobal
#include "SDLInputBridge.h"
#include "Platform.h"

#include <SDL2/SDL.h>
#include <iostream>

// Mock definitions for testing
HWND g_hWnd = nullptr;
HINSTANCE g_hInstance = nullptr;

class SubspaceApplication 
{
private:
    SDLGLObject* window_;
    SoundManager* soundManager_;
    SDLInputBridge* inputBridge_;
    bool running_;
    int windowWidth_;
    int windowHeight_;
    
    // Game state for testing
    float playerX_, playerY_;  // Simple player position
    bool thrustActive_;       // Thrust state
    bool leftTurn_, rightTurn_; // Turn states

public:
    SubspaceApplication() : 
        window_(nullptr), 
        soundManager_(nullptr),
        inputBridge_(nullptr),
        running_(false),
        windowWidth_(800),
        windowHeight_(600),
        playerX_(0.0f),
        playerY_(0.0f),
        thrustActive_(false),
        leftTurn_(false),
        rightTurn_(false)
    {
    }

    ~SubspaceApplication() 
    {
        cleanup();
    }

    bool initialize() 
    {
        std::cout << "Initializing Subspace Application..." << std::endl;

        // Initialize SDL2 and create window
        window_ = new SDLGLObject();
        if (!window_->init(32, windowWidth_, windowHeight_)) {
            std::cerr << "Failed to initialize SDL2 window!" << std::endl;
            return false;
        }

        // Initialize global settings
        subspaceGlobal.setWindowWidth(windowWidth_);
        subspaceGlobal.setWindowHeight(windowHeight_);

        // Initialize sound system
        if (!SoundManager::init()) {
            std::cerr << "Warning: Failed to initialize sound system" << std::endl;
            // Continue without sound
        } else {
            soundManager_ = new SoundManager();
        }
        
        // Initialize input bridge
        inputBridge_ = new SDLInputBridge();

        std::cout << "Subspace Application initialized successfully!" << std::endl;
        return true;
    }

    void cleanup() 
    {
        if (inputBridge_) {
            delete inputBridge_;
            inputBridge_ = nullptr;
        }
        
        if (soundManager_) {
            delete soundManager_;
            soundManager_ = nullptr;
        }
        
        SoundManager::destroy();
        
        if (window_) {
            window_->cleanup();
            delete window_;
            window_ = nullptr;
        }
    }

    void handleEvent(const SDL_Event& event) 
    {
        switch (event.type) {
            case SDL_QUIT:
                running_ = false;
                break;
                
            case SDL_KEYDOWN:
                handleKeyDown(event.key);
                break;
                
            case SDL_KEYUP:
                handleKeyUp(event.key);
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                handleMouseDown(event.button);
                break;
                
            case SDL_MOUSEBUTTONUP:
                handleMouseUp(event.button);
                break;
                
            case SDL_MOUSEMOTION:
                handleMouseMove(event.motion);
                break;
        }
    }

    void handleKeyDown(const SDL_KeyboardEvent& key) 
    {
        // Map SDL2 key to our input system
        InputEventType inputType = inputBridge_->mapKey(key.keysym.sym);
        
        bool shift, ctrl, alt;
        inputBridge_->getModifiers(key.keysym.mod, shift, ctrl, alt);
        
        std::cout << "Key pressed: " << SDL_GetKeyName(key.keysym.sym) 
                  << " (InputType: " << inputType << ")";
        if (shift) std::cout << " +SHIFT";
        if (ctrl) std::cout << " +CTRL";
        if (alt) std::cout << " +ALT";
        std::cout << std::endl;
        
        // Handle movement and actions
        switch (inputType) {
            case KEY_ESCAPE:
                running_ = false;
                break;
                
            case KEY_F1:
                std::cout << "F1 - Help: WASD/Arrows=move, Space=thrust, Mouse=fire, ESC=quit" << std::endl;
                break;
                
            case KEY_UP:
                thrustActive_ = true;
                std::cout << "THRUST ON" << std::endl;
                break;
                
            case KEY_LEFT:
                leftTurn_ = true;
                std::cout << "TURN LEFT" << std::endl;
                break;
                
            case KEY_RIGHT:
                rightTurn_ = true;
                std::cout << "TURN RIGHT" << std::endl;
                break;
                
            case ' ':  // Space bar
                std::cout << "SPECIAL ACTION (shields/bomb/etc.)" << std::endl;
                break;
                
            default:
                // Handle other keys
                if (inputType >= 32 && inputType < 127) {
                    std::cout << "ASCII key: '" << (char)inputType << "'" << std::endl;
                }
                break;
        }
    }

    void handleKeyUp(const SDL_KeyboardEvent& key) 
    {
        InputEventType inputType = inputBridge_->mapKey(key.keysym.sym);
        
        // Handle key releases for movement
        switch (inputType) {
            case KEY_UP:
                thrustActive_ = false;
                std::cout << "THRUST OFF" << std::endl;
                break;
                
            case KEY_LEFT:
                leftTurn_ = false;
                std::cout << "STOP TURN LEFT" << std::endl;
                break;
                
            case KEY_RIGHT:
                rightTurn_ = false;
                std::cout << "STOP TURN RIGHT" << std::endl;
                break;
        }
    }

    void handleMouseDown(const SDL_MouseButtonEvent& button) 
    {
        InputEventType inputType = inputBridge_->mapMouseButton(button.button);
        
        std::cout << "Mouse button " << (int)button.button << " pressed at (" 
                  << button.x << ", " << button.y << ") - InputType: " << inputType << std::endl;
                  
        switch (inputType) {
            case MOUSE_BUTTON_LEFT:
                std::cout << "FIRE PRIMARY WEAPON!" << std::endl;
                break;
            case MOUSE_BUTTON_RIGHT:
                std::cout << "FIRE SECONDARY WEAPON!" << std::endl;
                break;
            case MOUSE_BUTTON_MIDDLE:
                std::cout << "MIDDLE MOUSE ACTION" << std::endl;
                break;
        }
    }

    void handleMouseUp(const SDL_MouseButtonEvent& button) 
    {
        std::cout << "Mouse button " << (int)button.button << " released at (" 
                  << button.x << ", " << button.y << ")" << std::endl;
    }

    void handleMouseMove(const SDL_MouseMotionEvent& motion) 
    {
        // Only log if a button is pressed to avoid spam
        if (motion.state != 0) {
            std::cout << "Mouse moved to (" << motion.x << ", " << motion.y << ")" << std::endl;
        }
    }

    void update(float deltaTime) 
    {
        // Simple game logic for testing
        const float moveSpeed = 100.0f; // pixels per second
        const float turnSpeed = 180.0f; // degrees per second
        
        // Apply movement based on input state
        if (thrustActive_) {
            playerY_ -= moveSpeed * deltaTime; // Move up (negative Y)
        }
        
        if (leftTurn_) {
            playerX_ -= moveSpeed * deltaTime; // Move left
        }
        
        if (rightTurn_) {
            playerX_ += moveSpeed * deltaTime; // Move right  
        }
        
        // Keep player in bounds (simple wrapping)
        if (playerX_ < -1.0f) playerX_ = 1.0f;
        if (playerX_ > 1.0f) playerX_ = -1.0f;
        if (playerY_ < -1.0f) playerY_ = 1.0f;
        if (playerY_ > 1.0f) playerY_ = -1.0f;
    }

    void render() 
    {
        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Set clear color to dark blue for space background
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        
        // Render a simple ship representation
        glPushMatrix();
        glTranslatef(playerX_, playerY_, 0.0f);
        
        // Ship body (triangle)
        glBegin(GL_TRIANGLES);
        if (thrustActive_) {
            glColor3f(1.0f, 1.0f, 0.0f); // Yellow when thrusting
        } else {
            glColor3f(0.0f, 1.0f, 0.0f); // Green normally
        }
        glVertex2f(0.0f, 0.05f);    // Top point
        glVertex2f(-0.03f, -0.03f); // Bottom left
        glVertex2f(0.03f, -0.03f);  // Bottom right
        glEnd();
        
        // Thrust indicator
        if (thrustActive_) {
            glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.5f, 0.0f); // Orange thrust
            glVertex2f(-0.02f, -0.03f);
            glVertex2f(0.02f, -0.03f);
            glVertex2f(0.0f, -0.08f);
            glEnd();
        }
        
        glPopMatrix();
        
        // Render some UI text indication (just colored rectangles for now)
        if (leftTurn_) {
            glColor3f(1.0f, 0.0f, 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(-0.9f, 0.8f);
            glVertex2f(-0.8f, 0.8f);
            glVertex2f(-0.8f, 0.9f);
            glVertex2f(-0.9f, 0.9f);
            glEnd();
        }
        
        if (rightTurn_) {
            glColor3f(1.0f, 0.0f, 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(0.8f, 0.8f);
            glVertex2f(0.9f, 0.8f);
            glVertex2f(0.9f, 0.9f);
            glVertex2f(0.8f, 0.9f);
            glEnd();
        }
        
        // Swap buffers
        window_->swapBuffers();
    }

    void run() 
    {
        if (!initialize()) {
            return;
        }

        running_ = true;
        SDL_Event event;
        
        Uint32 lastTime = SDL_GetTicks();
        
        std::cout << "Starting main loop..." << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  WASD or Arrow Keys - Move ship" << std::endl;
        std::cout << "  Left Mouse - Fire primary weapon" << std::endl;
        std::cout << "  Right Mouse - Fire secondary weapon" << std::endl;
        std::cout << "  Space - Special action" << std::endl;
        std::cout << "  F1 - Help" << std::endl;
        std::cout << "  ESC - Quit" << std::endl;

        while (running_) {
            // Calculate delta time
            Uint32 currentTime = SDL_GetTicks();
            float deltaTime = (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            // Handle events
            while (SDL_PollEvent(&event)) {
                handleEvent(event);
            }

            // Update game logic
            update(deltaTime);

            // Render
            render();

            // Cap frame rate to ~60 FPS
            SDL_Delay(16);
        }

        std::cout << "Application shutting down..." << std::endl;
    }
};

int main(int argc, char* argv[]) 
{
    std::cout << "=== PSpace Subspace Client ===" << std::endl;
    std::cout << "SDL2-based cross-platform version" << std::endl;

    SubspaceApplication app;
    app.run();

    std::cout << "Application terminated." << std::endl;
    return 0;
}
