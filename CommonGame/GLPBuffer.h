#ifndef _GLPBUFFER_H_
#define _GLPBUFFER_H_

#include <windows.h>

#include <gl/glaux.h>

#include "DataTypes.h"

class GLPBuffer
{
public:
	GLPBuffer();
	~GLPBuffer();

	bool create(Uint width, Uint height);
	bool destroy();

	bool getRenderState(int timeout = 0);	//sets the texture for writing  //NOTE: changes viewport
	bool releaseRenderState();				//unsets the texture for writing

	bool bind();		//sets the this to the current texture
	bool unbind();		//unsets this as current texture

private:

	bool isLost();	//buffer lost from display mode change

	bool createBuffer(Uint width, Uint height);
	void destroyBuffer();

	bool setPixelFormat();

private:

	bool intialized_;
	int textureId_;

	HDC hDeviceContext_;
	HGLRC hRenderContext_;

	HPBUFFERARB hPBuffer_;
	HDC hPBufferDC_;
	HGLRC hPBufferRC_; 

	Uint width_, height_;
	int oldViewport_;
};

#endif