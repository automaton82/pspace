#ifndef _PLATFORM_H_
#define _PLATFORM_H_

// Platform detection
#ifdef _WIN32
    #define PLATFORM_WINDOWS
    #include <windows.h>
    #include <gl/gl.h>
    #include <gl/glu.h>
    #include <gl/glaux.h>
#else
    #define PLATFORM_LINUX
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_opengl.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
    
    // Windows compatibility types
    typedef unsigned int UINT;
    typedef unsigned char BYTE;
    typedef unsigned short WORD;
    typedef unsigned long DWORD;
    typedef int BOOL;
    typedef void* HANDLE;
    typedef void* HWND;
    typedef void* HDC;
    typedef void* HGLRC;
    typedef void* HINSTANCE;
    typedef char* LPSTR;
    typedef const char* LPCSTR;
    
    // Windows compatibility constants
    #define TRUE 1
    #define FALSE 0
    #ifndef NULL
    #define NULL 0
    #endif
    
    // Threading constants
    #define INFINITE 0xFFFFFFFF
    
    // Message box types (we'll need to implement these with SDL)
    #define MB_OK 0
    #define MB_ICONEXCLAMATION 0
    #define MB_ICONINFORMATION 0
    
    // Window styles (not used in SDL2 but needed for compatibility)
    #define WS_OVERLAPPEDWINDOW 0
    #define WS_CLIPSIBLINGS 0
    #define WS_CLIPCHILDREN 0
    #define WS_EX_APPWINDOW 0
    #define WS_EX_WINDOWEDGE 0
    
    // Virtual key codes (we'll map these to SDL)
    #define VK_ESCAPE SDLK_ESCAPE
    #define VK_SPACE SDLK_SPACE
    #define VK_LEFT SDLK_LEFT
    #define VK_UP SDLK_UP
    #define VK_RIGHT SDLK_RIGHT
    #define VK_DOWN SDLK_DOWN
    
    // GLaux compatibility (we'll implement these using SDL2_image)
    typedef struct {
        int width;
        int height;
        unsigned char* data;
    } AUX_IMAGE;
    
    // Windows bitmap structures (compatibility)
    typedef struct {
        WORD  bfType;
        DWORD bfSize;
        WORD  bfReserved1;
        WORD  bfReserved2;
        DWORD bfOffBits;
    } BITMAPFILEHEADER;
    
    typedef struct {
        DWORD biSize;
        DWORD biWidth;
        DWORD biHeight;
        WORD  biPlanes;
        WORD  biBitCount;
        DWORD biCompression;
        DWORD biSizeImage;
        DWORD biXPelsPerMeter;
        DWORD biYPelsPerMeter;
        DWORD biClrUsed;
        DWORD biClrImportant;
    } BITMAPINFOHEADER;
    
    typedef struct {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } RGBQUAD;
    
    // Windows file handling constants
    #define GENERIC_READ 0
    #define OPEN_EXISTING 0
    #define INVALID_HANDLE_VALUE ((HANDLE)-1)
    
    // Bitmap compression constants
    #define BI_RGB 0
    #define BI_RLE8 1
    #define BI_RLE4 2
    #define BI_BITFIELDS 3
    
    // Stub Windows API functions
    inline HANDLE CreateFile(const char* filename, DWORD access, DWORD shareMode, 
                            void* security, DWORD creation, DWORD flags, HANDLE templateFile) {
        return INVALID_HANDLE_VALUE; // Stub implementation
    }
    
    inline BOOL CloseHandle(HANDLE handle) {
        return TRUE; // Stub implementation
    }
    
    inline BOOL ReadFile(HANDLE file, void* buffer, DWORD bytesToRead, 
                        DWORD* bytesRead, void* overlapped) {
        if (bytesRead) *bytesRead = 0;
        return FALSE; // Stub implementation
    }
    
    // GLaux structures
    typedef struct {
        unsigned char* data;
        int sizeX, sizeY;
        int components;
    } AUX_RGBImageRec;
    
    // Stub for auxDIBImageLoad - will be implemented later
    AUX_RGBImageRec* auxDIBImageLoad(const char* filename);
    
#endif

#endif // _PLATFORM_H_