#include "GLObject.h"

#include "WinUtil.h"

GLObject::GLObject() :
	hDeviceContext_(NULL),
	hRenderContext_(NULL)
{
	renderMutex = CreateMutex(NULL, false, "packetMutex");
}

bool GLObject::init(int colorDepth, HWND hWnd)
{
	hWnd_ = hWnd;

	if(!createDeviceContext(hWnd))
		return false;

	if(!setPixelFormat(colorDepth))
		return false;

	if(!createRenderContext())
		return false;

	return true;
}

bool GLObject::cleanup()
{
	deleteDeviceContext();
	deleteRenderContext();

	return true;
}

const HDC GLObject::getDeviceContext() const
{
	return hDeviceContext_;
}

bool GLObject::createDeviceContext(HWND hWnd)
{
	hDeviceContext_ = GetDC(hWnd);
	if(!hDeviceContext_)
		return false;

	return true;
}

bool GLObject::deleteDeviceContext()
{
	if(!ReleaseDC(hWnd_, hDeviceContext_))
		return false;

	return true;
}

bool GLObject::setPixelFormat(int colorDepth)
{
	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		colorDepth,									// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	int pixelFormat = ChoosePixelFormat(hDeviceContext_, &pfd);
	if(pixelFormat == 0)
		return false;

	/*PIXELFORMATDESCRIPTOR check;
	DescribePixelFormat(hDeviceContext_, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &check);*/
  
	BOOL result = SetPixelFormat(hDeviceContext_, pixelFormat, &pfd);
	if(result == FALSE)
		return false;

    return true;
}

bool GLObject::createRenderContext()
{
	hRenderContext_ = wglCreateContext(hDeviceContext_);
	if(!hRenderContext_)
		return false;

	return true;
}


bool GLObject::deleteRenderContext()
{
	BOOL result = wglDeleteContext(hRenderContext_);
	if(result == TRUE)
		return true;

	return false;
}

bool GLObject::getRenderState(int time)
{
	DWORD result;
	DWORD timeout;

	if(!time)
		timeout = INFINITE;
	else
		timeout = time;

	result = WaitForSingleObject(renderMutex, timeout);
	if(result == WAIT_OBJECT_0)
	{
		hOldDeviceContext_ = wglGetCurrentDC();
		hOldRenderContext_ = wglGetCurrentContext();

		BOOL makeContext = wglMakeCurrent(hDeviceContext_, hRenderContext_);
		if(makeContext)
			return true;
	}
	
	return false;
}

bool GLObject::releaseRenderState()
{
	wglMakeCurrent(hOldDeviceContext_, hOldRenderContext_);

	BOOL result = ReleaseMutex(renderMutex);
	if(result == TRUE)
		return true;

	return false;
}

