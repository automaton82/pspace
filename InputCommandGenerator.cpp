#include "InputCommandGenerator.h"

#include <iostream>
#include <memory>
#include <SDL2/SDL.h>

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

std::vector<std::unique_ptr<InputCommand>> 
InputCommandGenerator::processKeyboardEvent(const SDL_KeyboardEvent& key, bool isPressed) 
{
    std::vector<std::unique_ptr<InputCommand>> commands;
    
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
            auto cmd = std::make_unique<InputCommands::InputActivated>();
            cmd->input = inputData;
            commands.push_back(std::move(cmd));
        } else if (!isPressed && wasPressed) {
            // Key just released - send InputUnactivated  
            auto cmd = std::make_unique<InputCommands::InputUnactivated>();
            cmd->input = inputData;
            commands.push_back(std::move(cmd));
        }
        
        if (isPressed) {
            // Key is currently held - send InputActive
            auto cmd = std::make_unique<InputCommands::InputActive>();
            cmd->input = inputData;
            commands.push_back(std::move(cmd));
        }
    }
    
    return commands;
}

std::vector<std::unique_ptr<InputCommand>>
InputCommandGenerator::processMouseButtonEvent(const SDL_MouseButtonEvent& button, bool isPressed) 
{
    std::vector<std::unique_ptr<InputCommand>> commands;
    
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
            auto cmd = std::make_unique<InputCommands::InputActivated>();
            cmd->input = inputData;
            commands.push_back(std::move(cmd));
        } else if (!isPressed && wasPressed) {
            auto cmd = std::make_unique<InputCommands::InputUnactivated>();
            cmd->input = inputData;
            commands.push_back(std::move(cmd));
        }
        
        if (isPressed) {
            auto cmd = std::make_unique<InputCommands::InputActive>();
            cmd->input = inputData;
            commands.push_back(std::move(cmd));
        }
    }
    
    return commands;
}

std::vector<std::unique_ptr<InputCommand>>
InputCommandGenerator::processMouseMotionEvent(const SDL_MouseMotionEvent& motion) 
{
    std::vector<std::unique_ptr<InputCommand>> commands;
    
    // For now, we don't generate commands for passive mouse movement
    // This can be added later if needed for mouse look, etc.
    
    return commands;
}

void InputCommandGenerator::executeCommands(const std::vector<std::unique_ptr<InputCommand>>& commands,
                                           InputCommandReceiver* receiver) 
{
    for (const auto& command : commands) {
        if (command) {
            // Execute with the provided receiver
            command->execute(receiver);
        }
    }
}

void InputCommandGenerator::debugPrintCommand(const InputCommand* command) const 
{
    if (!command) {
        std::cout << "NULL command" << std::endl;
        return;
    }
    
    // Since we don't have getTypeName in the real implementation, we'll print the address
    std::cout << "Command at: " << command << std::endl;
}
