#ifndef _GLEXTENSIONS_H_
#define _GLEXTENSIONS_H_

#include <gl/glext.h>
#include <gl/wglext.h>

//#include <wingdi.h>

//needs opengl32.lib

PFNWGLGETEXTENSIONSSTRINGARBPROC	wglGetExtensionsStringARB;

//pbuffer related
PFNWGLBINDTEXIMAGEARB				wglBindTexImageARB			= 0;
PFNWGLCHOOSEPIXELFORMATARBPROC		wglChoosePixelFormatARB		= 0;
PFNWGLDESTROYPBUFFERARB				wglDestroyPbufferARB		= 0;
PFNWGLGETPBUFFERDCARB				wglGetPbufferDCARB			= 0;
PFNWGLQUERYPBUFFERARB				wglQueryPbufferARB			= 0;
PFNWGLRELEASEPBUFFERDCARB			wglReleasePbufferDCARB		= 0;
PFNWGLSETPBUFFERATRRIBARB			wglSetPbufferAttribARB		= 0;

PFNGLRELEASETEXIMAGEARB				glReleaseTexImageARB;

bool initWGLExtensions(bool verbose = false);

bool isWglPBufferSupported();

#endif