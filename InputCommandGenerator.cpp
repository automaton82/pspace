#include "InputCommandGenerator.h"
#include "InputCommandsMock.h"
#include <iostream>
#include <SDL2/SDL.h>

// Implementation of mock InputCommands classes
namespace InputCommands {
    
    void InputActive::execute() {
        std::cout << "InputActive: " << input.type;
        if (input.shift) std::cout << " +SHIFT";
        if (input.ctrl) std::cout << " +CTRL"; 
        if (input.alt) std::cout << " +ALT";
        std::cout << std::endl;
    }
    
    void InputActivated::execute() {
        std::cout << "InputActivated: " << input.type;
        if (input.shift) std::cout << " +SHIFT";
        if (input.ctrl) std::cout << " +CTRL";
        if (input.alt) std::cout << " +ALT";
        std::cout << " (Key/Button PRESSED)" << std::endl;
    }
    
    void InputUnactivated::execute() {
        std::cout << "InputUnactivated: " << input.type;
        if (input.shift) std::cout << " +SHIFT";
        if (input.ctrl) std::cout << " +CTRL";
        if (input.alt) std::cout << " +ALT"; 
        std::cout << " (Key/Button RELEASED)" << std::endl;
    }
}

InputCommandGenerator::InputCommandGenerator() : inputBridge_(nullptr) 
{
    initializeStates();
}

InputCommandGenerator::~InputCommandGenerator() 
{
    // We don't own the inputBridge_, so don't delete it
}

void InputCommandGenerator::setInputBridge(SDLInputBridge* bridge) 
{
    inputBridge_ = bridge;
}

void InputCommandGenerator::initializeStates() 
{
    // Initialize state tracking arrays
    keyStates_.resize(512, false);  // Enough for most key codes
    mouseStates_.resize(8, false);  // Mouse buttons
}

InputCommands::InputData InputCommandGenerator::createInputData(InputEventType type, bool shift, bool ctrl, bool alt) const 
{
    return InputCommands::InputData(type, shift, ctrl, alt);
}

std::vector<std::unique_ptr<InputCommands::InputCommand>> 
InputCommandGenerator::processKeyboardEvent(const SDL_KeyboardEvent& key, bool isPressed) 
{
    std::vector<std::unique_ptr<InputCommands::InputCommand>> commands;
    
    if (!inputBridge_) {
        return commands;
    }
    
    // Map SDL2 key to our input type
    InputEventType inputType = inputBridge_->mapKey(key.keysym.sym);
    if (inputType == INPUT_UNKNOWN) {
        return commands;
    }
    
    // Get modifier states
    bool shift, ctrl, alt;
    inputBridge_->getModifiers(key.keysym.mod, shift, ctrl, alt);
    
    // Create input data
    InputCommands::InputData inputData = createInputData(inputType, shift, ctrl, alt);
    
    // Track state changes for proper activated/unactivated events
    int keyIndex = static_cast<int>(inputType);
    if (keyIndex >= 0 && keyIndex < static_cast<int>(keyStates_.size())) {
        bool wasPressed = keyStates_[keyIndex];
        keyStates_[keyIndex] = isPressed;
        
        if (isPressed && !wasPressed) {
            // Key just pressed - send InputActivated
            commands.push_back(std::unique_ptr<InputCommands::InputActivated>(new InputCommands::InputActivated(inputData)));
        } else if (!isPressed && wasPressed) {
            // Key just released - send InputUnactivated  
            commands.push_back(std::unique_ptr<InputCommands::InputUnactivated>(new InputCommands::InputUnactivated(inputData)));
        }
        
        if (isPressed) {
            // Key is currently held - send InputActive
            commands.push_back(std::unique_ptr<InputCommands::InputActive>(new InputCommands::InputActive(inputData)));
        }
    }
    
    return commands;
}

std::vector<std::unique_ptr<InputCommands::InputCommand>>
InputCommandGenerator::processMouseButtonEvent(const SDL_MouseButtonEvent& button, bool isPressed) 
{
    std::vector<std::unique_ptr<InputCommands::InputCommand>> commands;
    
    if (!inputBridge_) {
        return commands;
    }
    
    // Map SDL2 mouse button to our input type
    InputEventType inputType = inputBridge_->mapMouseButton(button.button);
    if (inputType == INPUT_UNKNOWN) {
        return commands;
    }
    
    // Get current keyboard modifier states
    Uint16 modState = SDL_GetModState();
    bool shift = (modState & KMOD_SHIFT) != 0;
    bool ctrl = (modState & KMOD_CTRL) != 0;
    bool alt = (modState & KMOD_ALT) != 0;
    
    InputCommands::InputData inputData = createInputData(inputType, shift, ctrl, alt);
    
    // Track mouse button states
    int buttonIndex = button.button;
    if (buttonIndex >= 0 && buttonIndex < static_cast<int>(mouseStates_.size())) {
        bool wasPressed = mouseStates_[buttonIndex];
        mouseStates_[buttonIndex] = isPressed;
        
        if (isPressed && !wasPressed) {
            commands.push_back(std::unique_ptr<InputCommands::InputActivated>(new InputCommands::InputActivated(inputData)));
        } else if (!isPressed && wasPressed) {
            commands.push_back(std::unique_ptr<InputCommands::InputUnactivated>(new InputCommands::InputUnactivated(inputData)));
        }
        
        if (isPressed) {
            commands.push_back(std::unique_ptr<InputCommands::InputActive>(new InputCommands::InputActive(inputData)));
        }
    }
    
    return commands;
}

std::vector<std::unique_ptr<InputCommands::InputCommand>>
InputCommandGenerator::processMouseMotionEvent(const SDL_MouseMotionEvent& motion) 
{
    std::vector<std::unique_ptr<InputCommands::InputCommand>> commands;
    
    // For now, we don't generate commands for passive mouse movement
    // This can be added later if needed for mouse look, etc.
    
    return commands;
}

void InputCommandGenerator::executeCommands(const std::vector<std::unique_ptr<InputCommands::InputCommand>>& commands,
                                           InputCommandReceiver* receiver) 
{
    for (const auto& command : commands) {
        if (command) {
            // For now, execute with null receiver since we're using mock commands
            command->execute();
        }
    }
}

void InputCommandGenerator::debugPrintCommand(const InputCommands::InputCommand* command) const 
{
    if (!command) {
        std::cout << "NULL command" << std::endl;
        return;
    }
    
    std::cout << "Command: " << command->getTypeName() 
              << " Input: " << command->input.type << std::endl;
}
