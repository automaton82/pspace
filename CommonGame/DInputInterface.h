#ifndef _DINPUTINTERFACE_H_
#define _DINPUTINTERFACE_H_

#ifdef _WIN32
#define DIRECTINPUT_VERSION 0x800
#include <windows.h>
#include <dinput.h>
#include <stdio.h>

#define KeyDown(data, n) ((data[n] & 0x80) ? true : false)
#define KeyUp(data, n) ((data[n] & 0x80) ? false : true)
#else
// Linux/SDL2 compatibility - DirectInput not available
// DInputInterface will be stubbed out
#include <cstdio>
typedef unsigned char BYTE;
typedef int HRESULT;
typedef void* LPDIRECTINPUT8;
typedef void* LPDIRECTINPUTDEVICE8;
typedef struct {
    long lX;
    long lY;
    long lZ;
    unsigned char rgbButtons[4];
} DIMOUSESTATE;
#define S_OK 0
#define KeyDown(data, n) false
#define KeyUp(data, n) true
#endif

class DInputInterface
{
public:
	bool Initialize(HWND hWnd, HINSTANCE hInstance);
	void Cleanup();

	bool Reacquire();

	const char* GetKeyStates();
	void GetKeyStates(char* keyState);	//256 chars
	DIMOUSESTATE* GetMouseState();

	char GetAsciiFromScanCode(int n);

private:
	bool InitDirectInput();
	bool InitKeyboard();
	bool InitMouse();

	HINSTANCE hInstance_;
	HWND hWnd_;

	LPDIRECTINPUT8 lpDI_;

	LPDIRECTINPUTDEVICE8 keyboard_;
	char keyState_[256];

	LPDIRECTINPUTDEVICE8 mouse_;
	DIMOUSESTATE mouseState_;
};

#endif
