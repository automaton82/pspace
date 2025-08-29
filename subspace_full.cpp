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
#include <GL/gl.h>
#include <GL/glu.h>  // For gluPerspective
#include <iostream>
#include <unistd.h>  // for chdir
#include <cmath>     // for atan

// Game function declarations
extern void GameDisplay();
extern void GameLoop();
extern void GameInit();
extern void GameDestroy();

// Mock Windows globals for compatibility
HWND g_hWnd = nullptr;
HINSTANCE g_hInstance = nullptr;

// Game components - use function to avoid static initialization order issues
SubspaceGameManager& getGameManager() {
    static SubspaceGameManager gameManager;
    return gameManager;
}

// Timers are defined in main-game.cpp
extern Timer displayTimer;
extern Timer gameTimer;

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
        std::cout << "Creating SDL window..." << std::endl;
        window_ = new SDLGLObject();
        if (!window_->init(32, windowWidth_, windowHeight_)) {
            std::cerr << "Failed to initialize SDL2 window!" << std::endl;
            return false;
        }
        std::cout << "SDL window created successfully" << std::endl;

        // Setup OpenGL viewport and perspective (essential for rendering)
        std::cout << "Setting up OpenGL viewport..." << std::endl;
        glViewport(0, 0, windowWidth_, windowHeight_);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        double ratio = (double)windowWidth_ / (double)windowHeight_;
        double fov = atan(windowHeight_ / 600.0) * 180.0 / 3.14159; // ~MathUtil::PI
        gluPerspective(fov, ratio, 10.0, 20000.0);
        
        glDepthRange(10, 20000);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Initialize OpenGL state (like the original InitGL function)
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glShadeModel(GL_SMOOTH);
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        
        glDisable(GL_LIGHTING);
        
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        
        glEnable(GL_TEXTURE_2D);
        
        std::cout << "OpenGL setup complete" << std::endl;

        // Initialize global settings
        std::cout << "Setting global window dimensions..." << std::endl;
        subspaceGlobal.setWindowWidth(windowWidth_);
        subspaceGlobal.setWindowHeight(windowHeight_);
        std::cout << "Global settings initialized" << std::endl;

        // Initialize sound system
        std::cout << "Initializing sound system..." << std::endl;
        if (!SoundManager::init()) {
            std::cerr << "Warning: Failed to initialize sound system" << std::endl;
        } else {
            soundManager_ = new SoundManager();
            std::cout << "Sound system initialized" << std::endl;
        }
        
        // Initialize input bridge
        std::cout << "Creating input bridge..." << std::endl;
        inputBridge_ = new SDLInputBridge();
        std::cout << "Input bridge created" << std::endl;

        // Initialize the real game manager using the proper game initialization
        try {
            std::cout << "Initializing game manager..." << std::endl;
            GameInit();
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
            GameDestroy();
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
            getGameManager().handleInput(inputEvent, mouseX, mouseY);
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
            
            getGameManager().handleInput(inputEvent, mouseX, mouseY);
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
            getGameManager().handleInput(inputEvent, button.x, button.y);
        }
    }

    void handleMouseUp(const SDL_MouseButtonEvent& button) 
    {
        InputEventType inputType = inputBridge_->mapMouseButton(button.button);
        
        if (gameInitialized_ && inputType != INPUT_UNKNOWN) {
            InputEvent inputEvent;
            inputEvent.type = inputType;
            inputEvent.state = INPUT_STATE_UP;
            
            getGameManager().handleInput(inputEvent, button.x, button.y);
        }
    }

    void handleMouseMove(const SDL_MouseMotionEvent& motion) 
    {
        // Only send passive mouse movement if game is initialized
        if (gameInitialized_) {
            InputEvent inputEvent;
            inputEvent.type = MOUSE_PASSIVE;
            inputEvent.state = INPUT_STATE_DOWN; // For movement events
            
            getGameManager().handleInput(inputEvent, motion.x, motion.y);
        }
    }

    void update(float deltaTime) 
    {
        if (gameInitialized_) {
            // Let the game manager handle updates
            getGameManager().update(deltaTime);
        }
    }

    void render() 
    {
        if (gameInitialized_) {
            // Let the game manager handle rendering
            getGameManager().display();
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
        
        std::cout << "Starting full Subspace game loop..." << std::endl;
        if (gameInitialized_) {
            std::cout << "Real Subspace game is running!" << std::endl;
        } else {
            std::cout << "Running in demo mode (game manager initialization failed)" << std::endl;
        }
        std::cout << "Press ESC to quit" << std::endl;

        while (running_) {
            // Handle SDL events first
            while (SDL_PollEvent(&event)) {
                handleEvent(event);
            }

            if (gameInitialized_) {
                // Call the real Subspace game loop which handles timing and rendering
                GameLoop();
            } else {
                // Fallback for demo mode
                update(0.016f);  // 60 FPS
                render();
                SDL_Delay(16);
            }
        }

        std::cout << "Subspace game shutting down..." << std::endl;
    }
};

// Global rendering function expected by main-game.cpp
void DrawGLScene()
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Push matrix for game rendering
    glPushMatrix();
    
    // Call the actual game display function
    extern void GameDisplay();
    GameDisplay();
    
    glPopMatrix();
    
    // Swap buffers to display the frame
    extern SDLGLObject& sdlGLObject;
    sdlGLObject.swapBuffers();
}

int main(int argc, char* argv[]) 
{
    std::cout << "=== PSpace - Full Subspace Game ===" << std::endl;
    std::cout << "SDL2-based cross-platform version" << std::endl;
    
    // Change to the correct directory where assets are located
    const char* assetDir = "../Subspace/subspace";
    if (chdir(assetDir) != 0) {
        std::cerr << "Warning: Could not change to asset directory: " << assetDir << std::endl;
        std::cerr << "Continuing with current directory..." << std::endl;
    } else {
        std::cout << "Changed to asset directory: " << assetDir << std::endl;
    }

    SubspaceGameApplication app;
    app.run();

    std::cout << "Game terminated." << std::endl;
    return 0;
}
