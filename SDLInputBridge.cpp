#include "SDLInputBridge.h"
#include <iostream>

SDLInputBridge::SDLInputBridge() 
{
    initializeKeyMapping();
}

void SDLInputBridge::initializeKeyMapping() 
{
    // Clear the map
    keyMap_.clear();
    
    // Basic movement keys
    keyMap_[SDLK_UP] = KEY_UP;
    keyMap_[SDLK_DOWN] = KEY_DOWN;
    keyMap_[SDLK_LEFT] = KEY_LEFT;
    keyMap_[SDLK_RIGHT] = KEY_RIGHT;
    
    // WASD movement (common for games)
    keyMap_[SDLK_w] = KEY_UP;
    keyMap_[SDLK_s] = KEY_DOWN;
    keyMap_[SDLK_a] = KEY_LEFT;
    keyMap_[SDLK_d] = KEY_RIGHT;
    
    // Function keys
    keyMap_[SDLK_F1] = KEY_F1;
    keyMap_[SDLK_F2] = KEY_F2;
    keyMap_[SDLK_F3] = KEY_F3;
    keyMap_[SDLK_F4] = KEY_F4;
    keyMap_[SDLK_F5] = KEY_F5;
    keyMap_[SDLK_F6] = KEY_F6;
    keyMap_[SDLK_F7] = KEY_F7;
    keyMap_[SDLK_F8] = KEY_F8;
    keyMap_[SDLK_F9] = KEY_F9;
    keyMap_[SDLK_F10] = KEY_F10;
    keyMap_[SDLK_F11] = KEY_F11;
    keyMap_[SDLK_F12] = KEY_F12;
    
    // Special keys
    keyMap_[SDLK_ESCAPE] = KEY_ESCAPE;
    keyMap_[SDLK_RETURN] = KEY_ENTER;
    keyMap_[SDLK_TAB] = KEY_TAB;
    keyMap_[SDLK_INSERT] = KEY_INSERT;
    keyMap_[SDLK_DELETE] = KEY_DELETE;
    keyMap_[SDLK_HOME] = KEY_HOME;
    keyMap_[SDLK_END] = KEY_END;
    keyMap_[SDLK_PAGEUP] = KEY_PAGEUP;
    keyMap_[SDLK_PAGEDOWN] = KEY_PAGEDOWN;
    
    // ASCII keys (a-z, 0-9, etc.) map directly to their ASCII values
    // These are handled specially in mapKey()
}

InputEventType SDLInputBridge::mapKey(SDL_Keycode sdlKey) const 
{
    // Check if it's in our special key map first
    auto it = keyMap_.find(sdlKey);
    if (it != keyMap_.end()) {
        return it->second;
    }
    
    // Handle ASCII keys (letters, numbers, symbols)
    if (sdlKey >= 32 && sdlKey < 127) {
        return static_cast<InputEventType>(sdlKey);
    }
    
    // Unknown key
    return INPUT_UNKNOWN;
}

InputEventType SDLInputBridge::mapMouseButton(Uint8 button) const 
{
    switch (button) {
        case SDL_BUTTON_LEFT:
            return MOUSE_BUTTON_LEFT;
        case SDL_BUTTON_RIGHT:
            return MOUSE_BUTTON_RIGHT;
        case SDL_BUTTON_MIDDLE:
            return MOUSE_BUTTON_MIDDLE;
        default:
            return INPUT_UNKNOWN;
    }
}

void SDLInputBridge::getModifiers(Uint16 kmod, bool& shift, bool& ctrl, bool& alt) const 
{
    shift = (kmod & KMOD_SHIFT) != 0;
    ctrl = (kmod & KMOD_CTRL) != 0;
    alt = (kmod & KMOD_ALT) != 0;
}

bool SDLInputBridge::isMovementKey(InputEventType inputType) const 
{
    switch (inputType) {
        case KEY_UP:
        case KEY_DOWN:
        case KEY_LEFT:
        case KEY_RIGHT:
            return true;
        default:
            return false;
    }
}

bool SDLInputBridge::isActionKey(InputEventType inputType) const 
{
    switch (inputType) {
        case MOUSE_BUTTON_LEFT:
        case MOUSE_BUTTON_RIGHT:
        case ' ':  // Space bar (common for shields/special)
        case 'x':  // X key (common for weapons)
        case 'z':  // Z key (common for weapons)
            return true;
        default:
            return false;
    }
}
