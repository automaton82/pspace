#include "GLPBuffer.h"

#include <assert.h>

#include "GLExtensions.h"
#include "WinUtil.h"

GLPBuffer::GLPBuffer() :
	hDeviceContext_(0),
	hRenderContext_(0),

	hPBuffer_(0), 
	hPBufferDC_(0),
	hPBufferRC_(0),

	initialized_(false),
	textureId_(-1),

	width_(0),
	height_(0)
{
}

GLPBuffer::~GLPBuffer()
{
	destroy();
}

bool GLPBuffer::create(Uint width, Uint height, Uint colorDepth)
{
	assert(isWglPBufferSupported());

	if(initialized_)
	{
		destroy();	
	}

	hDeviceContext_ = wglGetCurrentDC();
	if(!hDeviceContext_)
		return false;
	
	hRenderContext_ = wglGetCurrentContext();
	if(!hRenderContext_)
		return false;
	
	if(!setPixelFormat(colorDepth, zDepth))
		return false;
    
	if(!createBuffer(width, height))
		return false;

	width_ = width;
	height_ = height;
	initialized_ = true;

	return true;
}

bool GLPBuffer::destroy()
{
	assert(isWglPBufferSupported());

	if(!initialized_)
		return false;

	if(isLost())
		return false;
	
	destroyBuffer();
	
	initialized_ = false;

	return true;
}

bool GLPBuffer::getRenderState(int time)
{
	assert(isWglPBufferSupported());

	if(!initialized_)
		return false;

	if(isLost())
		return false;
	

	// set read/write context to pbuffer
	wglMakeContextCurrentARB(hPBufferDC_, hPBufferDC_, hPBufferRC_);
	
	glDrawBuffer(GL_FRONT);		//use front of buffer for draw and read
	glReadBuffer(GL_FRONT);
	
	glGetIntegerv(GL_VIEWPORT, oldViewport_);
	glViewport(0.0, 0.0, width_, height_);

	return true;	
}

bool GLPBuffer::releaseRenderState()
{
	assert(isWglPBufferSupported());

	if(!initialized_)
		return false;

	if(isLost())
		return false;
    
	wglMakeContextCurrentARB(hDeviceContext_, hDeviceContext_, hRenderContext_);	//set read/write context back to gl device

	glDrawBuffer(GL_BACK);	// draw to the back buffer of frame buffer
	glReadBuffer(GL_FRONT);	// read from the front buffer of the frame buffer

	glViewport(oldViewport_[0], oldViewport_[1], oldViewport_[2], oldViewport_[3]);	//reset the viewport

	return true;
}

bool GLPBuffer::bind()
{
	assert(isWglPBufferSupported());

	if(!intialized_)
		return false;

	if(isLost())
	{
		createBuffer(width_, height_);
	}
	
	// bind a texture object that we will associate a pbuffer with
	glBindTexture(GL_TEXTURE_2D, textureId_);

	// bind pbuffer to the currently bound texture object
	// while the pbuffer is bound to a texture it is illegal
	// to read or write from the pbuffer until it is released

	wglBindTexImageARB(hPBuffer_, WGL_FRONT_LEFT_ARB);
}


bool GLPBuffer::isLost()
{
	assert(isWglPBufferSupported());

	if(!intialized_)
		return false

	bool result = false;
 	int flag = 0;

	wglQueryPbufferARB(hPBuffer_, WGL_PBUFFER_LOST_ARB, &flag);
    if(flag != 0)		//error, lost
		result = true;

	return result;
}

bool GLPBuffer::createBuffer(Uint width, Uint height)
{
	assert(isWglPBufferSupported());

	// set some pbuffer attributes so that we can use this pbuffer as a
	// 2D RGBA texture target
	int pAttrib[] =	{
		WGL_TEXTURE_FORMAT_ARB, // our pbuffer will have texture format RGBA
		WGL_TEXTURE_RGBA_ARB,
		WGL_TEXTURE_TARGET_ARB, // of texture type GL_TEXTURE_2D
		WGL_TEXTURE_2D_ARB,
		0 // zero terminates the list
		};

	hPBuffer_ = wglCreatePbufferARB(hDeviceContext_, pixelFormat, width, height, pAttrib);
	hPBufferDC_ = wglGetPbufferDCARB(hPBuffer_);
	hPBufferRC_ = wglCreateContext(hPBufferDC_);

	glGenTextures(1, &textureId_);
	glBindTexture(GL_TEXTURE_2D, textureId_);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return true;
}

bool GLPBuffer::destroyBuffer()
{
	assert(isWglPBufferSupported());

	wglReleaseTexImageARB(hPBuffer_, WGL_FRONT_LEFT_ARB);
	wglDeleteContext(hPBufferRC_);
	wglReleasePbufferDCARB(hPBuffer_, hPBufferDC_);
	wglDestroyPbufferARB(hPBuffer_);
}

bool GLPBuffer::setPixelFormat()
{
	assert(isWglPBufferSupported());

	//TODO: assume off-screen rendering?

	bool result = false;
	Uint numFormats = 0;
	
	int iAttr[] = {
		WGL_SUPPORT_OPENGL_ARB, TRUE, // pbuffer will be used with gl
		WGL_DRAW_TO_PBUFFER_ARB, TRUE, // enable render to pbuffer
		WGL_BIND_TO_TEXTURE_RGBA_ARB, TRUE, // pbuffer will be used as a texture
		WGL_RED_BITS_ARB, 8, // at least 8 bits for RED channel
		WGL_GREEN_BITS_ARB, 8, // at least 8 bits for GREEN channel
		WGL_BLUE_BITS_ARB, 8, // at least 8 bits for BLUE channel
		WGL_ALPHA_BITS_ARB, 8, // at least 8 bits for ALPHA channel
		WGL_DEPTH_BITS_ARB, 24, // at least 24 bits for depth buffer		//TODO: allow color depth to be specified?
		WGL_DOUBLE_BUFFER_ARB, FALSE, // we don’t require double buffering
		0 // zero terminates the list
		};

	result = wglChoosePixelFormatARB(hDeviceContext_, iAttr, 0, 1, &pixelFormat_, &numFormats);
	
	return result;
}
