#ifndef _GLOBJECT_H_
#define _GLOBJECT_H_

#include <windows.h>

//#include <gl/gl.h>
//#include <gl/glu.h>
#include <gl/glaux.h>

#include "DataTypes.h"
#include "Singleton.h"

class GLObject
{
public:
	GLObject();
	~GLObject();

	bool init(int colorDepth = 32, HWND hWnd = NULL);
	bool cleanup();

	bool getRenderState(int timeout = 0);
	bool releaseRenderState();

	const HDC getDeviceContext() const;

private:
	bool createDeviceContext(HWND hWnd);
	bool deleteDeviceContext();

	bool setPixelFormat(int colorDepth);

	bool createRenderContext();
	bool deleteRenderContext();

	HWND hWnd_;
	HDC hDeviceContext_;
	HGLRC hRenderContext_;

	HDC hOldDeviceContext_;
	HGLRC hOldRenderContext_;


	HANDLE renderMutex;
};

static GLObject& glObject = *Singleton<GLObject>::instance();

#endif