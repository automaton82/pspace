#include "SDLGLObject.h"
#include "SoundManager.h"
#include "SDLInputBridge.h"
#include "Platform.h"

// Real Subspace game components
#include "SubspaceGameManager.h"
#include "SubspaceGlobal.h"
#include "Timer.h"
#include "InputEvent.h"

#include <SDL2/SDL.h>
#include <iostream>

// Mock Windows globals for compatibility
HWND g_hWnd = nullptr;
HINSTANCE g_hInstance = nullptr;

// Game components
static SubspaceGameManager gameManager;
Timer displayTimer;
Timer gameTimer;

class SubspaceGameApplication 
{
private:
    SDLGLObject* window_;
    SoundManager* soundManager_;
    SDLInputBridge* inputBridge_;
    bool running_;
    int windowWidth_;
    int windowHeight_;
    bool gameInitialized_;

public:
    SubspaceGameApplication() : 
        window_(nullptr), 
        soundManager_(nullptr),
        inputBridge_(nullptr),
        running_(false),
        windowWidth_(800),
        windowHeight_(600),
        gameInitialized_(false)
    {
    }

    ~SubspaceGameApplication() 
    {
        cleanup();
    }

    bool initialize() 
    {
        std::cout << "Initializing Full Subspace Game..." << std::endl;

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
        } else {
            soundManager_ = new SoundManager();
        }
        
        // Initialize input bridge
        inputBridge_ = new SDLInputBridge();

        // Initialize the real game manager
        try {
            std::cout << "Initializing game manager..." << std::endl;
            gameManager.init();
            gameTimer.start();
            displayTimer.start();
            gameInitialized_ = true;
            std::cout << "Game manager initialized successfully!" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Failed to initialize game manager: " << e.what() << std::endl;
            std::cerr << "Continuing with basic demo mode..." << std::endl;
            gameInitialized_ = false;
        }

        std::cout << "Subspace Game Application initialized!" << std::endl;
        return true;
    }

    void cleanup() 
    {
        if (gameInitialized_) {
            gameManager.destroy();
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
        InputEventType inputType = inputBridge_->mapKey(key.keysym.sym);
        
        if (inputType == KEY_ESCAPE) {
            running_ = false;
            return;
        }

        // Create InputEvent for the game manager
        if (gameInitialized_ && inputType != INPUT_UNKNOWN) {
            InputEvent inputEvent;
            inputEvent.type = inputType;
            inputEvent.state = INPUT_STATE_DOWN;
            
            // Get mouse position for the input event
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            
            std::cout << "Game Input: " << inputType << " (Key: " << SDL_GetKeyName(key.keysym.sym) << ")" << std::endl;
            gameManager.handleInput(inputEvent, mouseX, mouseY);
        }
    }

    void handleKeyUp(const SDL_KeyboardEvent& key) 
    {
        InputEventType inputType = inputBridge_->mapKey(key.keysym.sym);
        
        if (gameInitialized_ && inputType != INPUT_UNKNOWN) {
            InputEvent inputEvent;
            inputEvent.type = inputType;
            inputEvent.state = INPUT_STATE_UP;
            
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            
            gameManager.handleInput(inputEvent, mouseX, mouseY);
        }
    }

    void handleMouseDown(const SDL_MouseButtonEvent& button) 
    {
        InputEventType inputType = inputBridge_->mapMouseButton(button.button);
        
        if (gameInitialized_ && inputType != INPUT_UNKNOWN) {
            InputEvent inputEvent;
            inputEvent.type = inputType;
            inputEvent.state = INPUT_STATE_DOWN;
            
            std::cout << "Game Mouse Input: " << inputType << " at (" << button.x << ", " << button.y << ")" << std::endl;
            gameManager.handleInput(inputEvent, button.x, button.y);
        }
    }

    void handleMouseUp(const SDL_MouseButtonEvent& button) 
    {
        InputEventType inputType = inputBridge_->mapMouseButton(button.button);
        
        if (gameInitialized_ && inputType != INPUT_UNKNOWN) {
            InputEvent inputEvent;
            inputEvent.type = inputType;
            inputEvent.state = INPUT_STATE_UP;
            
            gameManager.handleInput(inputEvent, button.x, button.y);
        }
    }

    void handleMouseMove(const SDL_MouseMotionEvent& motion) 
    {
        // Only send passive mouse movement if game is initialized
        if (gameInitialized_) {
            InputEvent inputEvent;
            inputEvent.type = MOUSE_PASSIVE;
            inputEvent.state = INPUT_STATE_DOWN; // For movement events
            
            gameManager.handleInput(inputEvent, motion.x, motion.y);
        }
    }

    void update(float deltaTime) 
    {
        if (gameInitialized_) {
            // Let the game manager handle updates
            gameManager.handleGameUpdate();
        }
    }

    void render() 
    {
        if (gameInitialized_) {
            // Let the game manager handle rendering
            gameManager.handleDisplay();
        } else {
            // Fallback rendering if game manager failed to initialize
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
            
            // Simple "loading" indicator
            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_QUADS);
            glVertex2f(-0.1f, -0.02f);
            glVertex2f(0.1f, -0.02f);
            glVertex2f(0.1f, 0.02f);
            glVertex2f(-0.1f, 0.02f);
            glEnd();
        }
        
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
        
        std::cout << "Starting full Subspace game loop..." << std::endl;
        if (gameInitialized_) {
            std::cout << "Real Subspace game is running!" << std::endl;
        } else {
            std::cout << "Running in demo mode (game manager initialization failed)" << std::endl;
        }
        std::cout << "Press ESC to quit" << std::endl;

        while (running_) {
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

            // Cap frame rate
            SDL_Delay(16);
        }

        std::cout << "Subspace game shutting down..." << std::endl;
    }
};

int main(int argc, char* argv[]) 
{
    std::cout << "=== PSpace - Full Subspace Game ===" << std::endl;
    std::cout << "SDL2-based cross-platform version" << std::endl;

    SubspaceGameApplication app;
    app.run();

    std::cout << "Game terminated." << std::endl;
    return 0;
}
