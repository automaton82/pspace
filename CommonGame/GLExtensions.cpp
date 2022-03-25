#include "GLExtensions.h"

#include <iostream>
using namespace std;

bool isExtSupported(const char* extStr, const char* s)
{
	char result = strstr(extStr, s);

	return (result != 0);
}

bool initWGLExtensions(bool verbose)
{
	bool retval = true;
	const char* extensionsStr = 0;
	const char* glExtStr = 0;

	glExtStr = (char*)glGetString(GL_EXTENSIONS);

	wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress(“wglGetExtensionsStringARB”);
	if(wglGetExtrensionsStringARB == 0)
	{
		cout << "Error: wglGetExtensionsStringARB not supported." << endl;
		return false;
	}

	extensionsStr = wglGetExtensionsStringARB(wglGetCurrentDC());

	if(verbose)
	{
		cout << "Supported extensions: " << endl;
		cout << extensionsStr << endl;
	}

#define checkWGLSupport(funcName, funcType) \
	if(!isExtSupported(extensionsStr, funcName) && !isExtSupported(glExtStr, funcName)) \
		if(verbose) cout << "Extension string not found, " << funcName << endl; \
	funcName = (funcType)wglGetProcAddress("funcName"); \
	if(funcName == 0) \
		if(verbose) cout << "Extension not supported, " << funcName << endl; \
	else \
		if(verbose) cout << "Extension found, " << funcName << endl;

	//pbuffer related stuff

	checkWGLSupport(wglBindTexImageARB, PFNWGLBINDTEXIMAGEARB);
	checkWGLSupport(wglChoosePixelFormatARB, PFNWGLCHOOSEPIXELFORMATARBPROC);
	checkWGLSupport(wglDestroyPbufferARB, PFNWGLDESTROYPBUFFERARB);
	checkWGLSupport(wglGetPbufferDCARB, PFNWGLGETPBUFFERDCARB);
	checkWGLSupport(wglQueryPbufferARB, PFNWGLQUERYPBUFFERARB);
	checkWGLSupport(wglReleasePbufferDCARB, PFNWGLRELEASEPBUFFERDCARB);
	checkWGLSupport(wglSetPbufferAttribARB, PFNWGLSETPBUFFERATRRIBARB);

	checkWGLSupport(glReleaseTexImageARB, PFNGLRELEASETEXIMAGEARB);

	/*wglBindTexImageARB = (PFNWGLBINDTEXIMAGEARB)wglGetProcAddress("wglBindTexImageARB");
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARB)wglGetProcAddress("wglDestroyPbufferARB");
	wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARB)wglGetProcAddress("wglGetPbufferDCARB");
	wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARB)wglGetProcAddress("wglQueryPbufferARB");
    wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARB)wglGetProcAddress("wglReleasePbufferDCARB");
    wglSetPbufferAttribARB = (PFNWGLSETPBUFFERATRRIBARB)wglGetProcAddress("wglSetPbufferAttribARB");

    glReleaseTexImageARB = (PFNGLRELEASETEXIMAGEARB)wglGetProcAddress("glReleaseTexImageARB");*/
	
	return true;
}

bool isWglPBufferSupported()
{
	bool retval = false;

	if(wglBindTexImageARB && 
		wglChoosePixelFormatARB && 
		wglDestroyPbufferARB &&
		wglGetPbufferDCARB &&
		wglQueryPbufferARB &&
		wglReleasePbufferDCARB &&
		wglSetPbufferAttribARB)
		retval = true;

	return retval;
}