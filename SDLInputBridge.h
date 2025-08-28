#ifndef _SDL_INPUT_BRIDGE_H_
#define _SDL_INPUT_BRIDGE_H_

#include <SDL2/SDL.h>
#include "InputEvent.h"
#include <map>

/**
 * Bridge between SDL2 events and Subspace InputEvent system
 * Converts SDL2 keyboard/mouse events to InputEventType enums
 */
class SDLInputBridge 
{
private:
    std::map<SDL_Keycode, InputEventType> keyMap_;
    
    void initializeKeyMapping();
    
public:
    SDLInputBridge();
    
    /**
     * Convert SDL2 keyboard event to InputEventType
     * @param sdlKey SDL2 key code
     * @return Corresponding InputEventType or INPUT_UNKNOWN
     */
    InputEventType mapKey(SDL_Keycode sdlKey) const;
    
    /**
     * Convert SDL2 mouse button to InputEventType
     * @param button SDL2 mouse button (SDL_BUTTON_LEFT, etc.)
     * @return Corresponding InputEventType or INPUT_UNKNOWN
     */
    InputEventType mapMouseButton(Uint8 button) const;
    
    /**
     * Get modifier state from SDL2 keyboard state
     * @param kmod SDL2 keyboard modifiers
     * @param shift Output: true if shift pressed
     * @param ctrl Output: true if ctrl pressed  
     * @param alt Output: true if alt pressed
     */
    void getModifiers(Uint16 kmod, bool& shift, bool& ctrl, bool& alt) const;
    
    /**
     * Check if a key represents movement input
     * @param inputType The input type to check
     * @return true if this is a movement key
     */
    bool isMovementKey(InputEventType inputType) const;
    
    /**
     * Check if a key represents weapon/action input
     * @param inputType The input type to check
     * @return true if this is a weapon/action key
     */
    bool isActionKey(InputEventType inputType) const;
};

#endif
