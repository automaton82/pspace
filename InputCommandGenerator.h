#ifndef _INPUT_COMMAND_GENERATOR_H_
#define _INPUT_COMMAND_GENERATOR_H_

#include "InputEvent.h"
#include "SDLInputBridge.h"
#include <vector>
#include <memory>

// Forward declarations to avoid including the problematic files for now
namespace InputCommands {
    struct InputData;
    class InputCommand;
    class InputActive;
    class InputActivated;
    class InputUnactivated;
}

class InputCommandReceiver;

/**
 * Generates InputCommand objects from SDL2 input events
 * Bridges SDL2 → InputEventType → InputCommand → Game Actions
 */
class InputCommandGenerator 
{
private:
    SDLInputBridge* inputBridge_;
    
    // Track input states for proper activated/unactivated events
    std::vector<bool> keyStates_;
    std::vector<bool> mouseStates_;
    
    void initializeStates();
    InputCommands::InputData createInputData(InputEventType type, bool shift, bool ctrl, bool alt) const;
    
public:
    InputCommandGenerator();
    ~InputCommandGenerator();
    
    /**
     * Initialize with an input bridge
     */
    void setInputBridge(SDLInputBridge* bridge);
    
    /**
     * Process SDL2 keyboard event and generate appropriate InputCommands
     * @param key SDL2 keyboard event
     * @param isPressed true for key down, false for key up
     * @return vector of InputCommand objects to execute
     */
    std::vector<std::unique_ptr<InputCommands::InputCommand>> 
        processKeyboardEvent(const SDL_KeyboardEvent& key, bool isPressed);
    
    /**
     * Process SDL2 mouse button event and generate appropriate InputCommands  
     * @param button SDL2 mouse button event
     * @param isPressed true for button down, false for button up
     * @return vector of InputCommand objects to execute
     */
    std::vector<std::unique_ptr<InputCommands::InputCommand>>
        processMouseButtonEvent(const SDL_MouseButtonEvent& button, bool isPressed);
    
    /**
     * Process SDL2 mouse motion and generate passive input if needed
     * @param motion SDL2 mouse motion event
     * @return vector of InputCommand objects to execute (usually empty or passive movement)
     */
    std::vector<std::unique_ptr<InputCommands::InputCommand>>
        processMouseMotionEvent(const SDL_MouseMotionEvent& motion);
    
    /**
     * Execute a list of input commands on a receiver
     * @param commands List of commands to execute
     * @param receiver Target receiver for the commands
     */
    void executeCommands(const std::vector<std::unique_ptr<InputCommands::InputCommand>>& commands,
                        InputCommandReceiver* receiver);
    
    /**
     * Debug: Print information about an input command
     */
    void debugPrintCommand(const InputCommands::InputCommand* command) const;
};

#endif
