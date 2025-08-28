#include "SDLGLObject.h"
#include "SoundManager.h"
#include "SubspaceMock.h"  // Use mock instead of full SubspaceGlobal
#include "SDLInputBridge.h"
#include "InputCommandGenerator.h"
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
    InputCommandGenerator* commandGenerator_;
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
        commandGenerator_(nullptr),
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
        
        // Initialize input command generator
        commandGenerator_ = new InputCommandGenerator();
        commandGenerator_->setInputBridge(inputBridge_);

        std::cout << "Subspace Application initialized successfully!" << std::endl;
        return true;
    }

    void cleanup() 
    {
        if (commandGenerator_) {
            delete commandGenerator_;
            commandGenerator_ = nullptr;
        }
        
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
        // Generate input commands using the command generator
        auto commands = commandGenerator_->processKeyboardEvent(key, true);
        
        // Execute the commands (for now, they just print debug info)
        commandGenerator_->executeCommands(commands, nullptr);
        
        // Also handle some direct game logic for visual feedback
        InputEventType inputType = inputBridge_->mapKey(key.keysym.sym);
        switch (inputType) {
            case KEY_ESCAPE:
                running_ = false;
                break;
                
            case KEY_F1:
                std::cout << "=== INPUT COMMAND SYSTEM DEMO ===" << std::endl;
                std::cout << "This demonstrates SDL2 → InputCommand integration" << std::endl;
                std::cout << "Watch the console for InputActivated/InputActive/InputUnactivated events" << std::endl;
                break;
                
            case KEY_UP:
                thrustActive_ = true;
                break;
                
            case KEY_LEFT:
                leftTurn_ = true;
                break;
                
            case KEY_RIGHT:
                rightTurn_ = true;
                break;
        }
    }

    void handleKeyUp(const SDL_KeyboardEvent& key) 
    {
        // Generate input commands for key release
        auto commands = commandGenerator_->processKeyboardEvent(key, false);
        commandGenerator_->executeCommands(commands, nullptr);
        
        // Handle direct game logic
        InputEventType inputType = inputBridge_->mapKey(key.keysym.sym);
        switch (inputType) {
            case KEY_UP:
                thrustActive_ = false;
                break;
                
            case KEY_LEFT:
                leftTurn_ = false;
                break;
                
            case KEY_RIGHT:
                rightTurn_ = false;
                break;
        }
    }

    void handleMouseDown(const SDL_MouseButtonEvent& button) 
    {
        // Generate mouse input commands
        auto commands = commandGenerator_->processMouseButtonEvent(button, true);
        commandGenerator_->executeCommands(commands, nullptr);
        
        // Also show mouse position info
        std::cout << "Mouse click at (" << button.x << ", " << button.y << ")" << std::endl;
    }

    void handleMouseUp(const SDL_MouseButtonEvent& button) 
    {
        // Generate mouse release commands
        auto commands = commandGenerator_->processMouseButtonEvent(button, false);
        commandGenerator_->executeCommands(commands, nullptr);
    }

    void handleMouseMove(const SDL_MouseMotionEvent& motion) 
    {
        // Generate mouse motion commands if needed
        auto commands = commandGenerator_->processMouseMotionEvent(motion);
        commandGenerator_->executeCommands(commands, nullptr);
        
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
        std::cout << "=== INPUT COMMAND SYSTEM INTEGRATION TEST ===" << std::endl;
        std::cout << "This demonstrates the complete input pipeline:" << std::endl;
        std::cout << "SDL2 Events → SDLInputBridge → InputCommands → Game Actions" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  WASD or Arrow Keys - Move ship" << std::endl;
        std::cout << "  Left/Right Mouse - Fire weapons" << std::endl;
        std::cout << "  Space - Special action" << std::endl;
        std::cout << "  F1 - Show system info" << std::endl;
        std::cout << "  ESC - Quit" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Watch console output for InputCommand events!" << std::endl;

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
