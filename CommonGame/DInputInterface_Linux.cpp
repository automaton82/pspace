#include "DInputInterface.h"

// Linux stub implementation for DInputInterface
// TODO: Replace with SDL2-based input handling

bool DInputInterface::Initialize(HWND hwnd, HINSTANCE hinstance)
{
    // Stub - return true (success) on Linux
    return true;
}

void DInputInterface::Cleanup()
{
    // Stub - no-op on Linux
}

bool DInputInterface::Reacquire()
{
    // Stub - return true (success) on Linux
    return true;
}

const char* DInputInterface::GetKeyStates()
{
    // Stub - return empty key state on Linux
    static char emptyKeys[256] = {0};
    return emptyKeys;
}

void DInputInterface::GetKeyStates(char* keyState)
{
    // Stub - clear key state on Linux
    for (int i = 0; i < 256; i++) {
        keyState[i] = 0;
    }
}

DIMOUSESTATE* DInputInterface::GetMouseState()
{
    // Stub - return nullptr on Linux
    return nullptr;
}

char DInputInterface::GetAsciiFromScanCode(int n)
{
    // Stub - return null character on Linux
    return '\0';
}
