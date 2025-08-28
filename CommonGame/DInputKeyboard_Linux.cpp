#include "DInputKeyboard.h"

// Linux stub implementation for DInputKeyboard
// TODO: Replace with SDL2-based input handling

DInputKeyboard::DInputKeyboard(DInputInterface* dinputInterface)
    : inputInterface_(dinputInterface), callbackFunc_(nullptr)
{
}

DInputKeyboard::~DInputKeyboard()
{
}

void DInputKeyboard::setCallback(CallbackFunc callback)
{
    // Store callback for future use
    callbackFunc_ = callback;
}

void DInputKeyboard::refresh()
{
    // Stub - no-op on Linux
}

const char* DInputKeyboard::getKeyStates()
{
    // Stub - return empty key state on Linux
    static char emptyKeys[256] = {0};
    return emptyKeys;
}

void DInputKeyboard::getKeyStates(char* keyState)
{
    // Stub - clear key state on Linux
    for (int i = 0; i < 256; i++) {
        keyState[i] = 0;
    }
}

const char* DInputKeyboard::getAsciiKeyStates()
{
    // Stub - return empty ASCII key state on Linux
    static char emptyAsciiKeys[256] = {0};
    return emptyAsciiKeys;
}

InputEventType DInputKeyboard::getInputFromAscii(char c)
{
    // Stub - return no input on Linux
    return INPUT_UNKNOWN;
}

InputEventType DInputKeyboard::getInputFromScanCode(int n)
{
    // Stub - return no input on Linux
    return INPUT_UNKNOWN;
}
