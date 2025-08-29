#ifndef _MAIN_H_
#define _MAIN_H_

#include "Platform.h"

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define STRICT

#include <windows.h>		// Header File For Windows

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#else
// Linux compatibility
typedef void* HWND;
typedef void* HINSTANCE;
typedef void* HDC;
typedef void* HGLRC;
typedef unsigned int DWORD;  // Use consistent 32-bit definition
typedef long LRESULT;        // Add missing LRESULT type
typedef unsigned int UINT;
typedef unsigned long WPARAM;
typedef long LPARAM;
typedef int BOOL;
typedef void* WNDPROC;

// Windows constants
#define TRUE 1
#define FALSE 0
#define CALLBACK  // Empty macro for Linux
#define WINAPI    // Empty macro for Linux

// Avoid redefinition warnings - use Platform.h values
#ifndef MB_OK
#define MB_OK          0x00000000L
#endif
#ifndef MB_ICONEXCLAMATION  
#define MB_ICONEXCLAMATION 0x00000030L
#endif

// Windows message constants - stub values
#define WM_ACTIVATE    0x0006
#define WM_COMMAND     0x0111
#define WM_CREATE      0x0001
#define WM_PAINT       0x000F
#define WM_SIZE        0x0005
#define WM_CLOSE       0x0010
#define WM_DESTROY     0x0002

// Windows style constants - stub values  
#define CS_HREDRAW     0x0002
#define CS_VREDRAW     0x0001
#define CS_OWNDC       0x0020
#define SW_SHOW        5

// Message handling constants
#define PM_NOREMOVE    0x0000

// Resource constants
#define IDI_WINLOGO    32517
#define IDC_ARROW      32512

// Other types  
typedef char* LPSTR;

typedef struct tagMSG {
    HWND hwnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD time;
    // Simplified structure
} MSG;
typedef struct tagPAINTSTRUCT {
    HDC hdc;
    BOOL fErase;
    // Simplified structure
} PAINTSTRUCT;

typedef struct tagRECT {
    long left;
    long top;
    long right;
    long bottom;
} RECT;

typedef struct tagWNDCLASS {
    UINT style;
    WNDPROC lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    HINSTANCE hInstance;
    void* hIcon;
    void* hCursor;
    void* hbrBackground;
    const char* lpszMenuName;
    const char* lpszClassName;
} WNDCLASS;

// Linux stub functions
inline void* GetModuleHandle(void*) { return nullptr; }
inline void* LoadIcon(void*, int) { return nullptr; }
inline void* LoadCursor(void*, int) { return nullptr; }
inline int RegisterClass(WNDCLASS*) { return 1; }
inline int MessageBox(void*, const char*, const char*, int) { return 0; }
inline void AdjustWindowRectEx(RECT*, DWORD, BOOL, DWORD) {}
inline void* CreateWindowEx(DWORD, const char*, const char*, DWORD, int, int, int, int, HWND, void*, HINSTANCE, void*) { return nullptr; }
inline int ShowWindow(HWND, int) { return 0; }
inline int SetForegroundWindow(HWND) { return 0; }
inline HWND SetFocus(HWND) { return nullptr; }
inline int DestroyWindow(HWND) { return 1; }
inline int UnregisterClass(const char*, HINSTANCE) { return 1; }
inline HDC BeginPaint(HWND, PAINTSTRUCT*) { return nullptr; }
inline int EndPaint(HWND, PAINTSTRUCT*) { return 0; }
inline LRESULT SendMessage(HWND, UINT, WPARAM, LPARAM) { return 0; }
inline void PostQuitMessage(int) {}
inline LRESULT DefWindowProc(HWND, UINT, WPARAM, LPARAM) { return 0; }
inline int SwapBuffers(HDC) { return 0; }

// Message handling stubs
inline int PeekMessage(MSG*, HWND, UINT, UINT, UINT) { return 0; }
inline int GetMessage(MSG*, HWND, UINT, UINT) { return 0; }
inline int TranslateMessage(MSG*) { return 1; }
inline LRESULT DispatchMessage(MSG*) { return 0; }
inline void WaitMessage() {}

#include <GL/gl.h>
#include <GL/glu.h>
// glaux is deprecated, skip it

//Window (Linux stubs)
LRESULT CALLBACK WndProc(HWND, UINT,WPARAM, LPARAM);

//Display
bool CreateGLWindow(char* title, int width, int height, int bits);
void DrawGLScene();
void KillGLWindow();							// Properly Kill The Window
void ReSizeGLScene(int width, int height);		// Resize And Initialize The GL Window
bool InitGL();

#endif  // _WIN32

#include "InputEvent.h"

//Game 
void GameInit();
void GameDestroy();
void GameInput(InputEvent input);
void GameLoop();
void GameTick(double value);
void GameResize(int width, int height);
void GameDisplay();


#endif