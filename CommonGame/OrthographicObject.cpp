#include "OrthographicObject.h"

#include <windows.h>
#include <gl/gl.h>

OrthographicObject::OrthographicObject() :
	orthoX_(0), orthoY_(0),
	orthoWidth_(800), orthoHeight_(600),
	orthoNear_(10.0), orthoFar_(20000.0)
{
	//initialize with actual window values
}

OrthographicObject::~OrthographicObject()
{
}

void OrthographicObject::setOrthoMode(double x, double y, double width, double height, double nearClip, double farClip)
{
	orthoX_ = x;
	orthoY_ = y;
	orthoWidth_ = width;
	orthoHeight_ = height;
	orthoNear_ = nearClip;
	orthoFar_ = farClip;
}

void OrthographicObject::enterOrthoMode() const
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();

		//VideoData& video = VideoData::getInstance();
		//glOrtho(0, video.windowWidth, 0, video.windowHeight, -1, 1);
		glOrtho(orthoX_, orthoX_+orthoWidth_, orthoY_, orthoY_+orthoHeight_, orthoNear_, orthoFar_);		//TODO: use actual window size
		glDepthRange(orthoNear_, orthoFar_);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();

			//glDepthMask(true);
			//glDepthFunc(GL_GEQUAL);
			//draw

	//exit ortho mode
}

void OrthographicObject::exitOrthoMode() const
{
	//enter ortho mode
		//draw
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void OrthographicObject::display() const
{
	enterOrthoMode();

	this->draw();

	exitOrthoMode();

	/*glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();

		//VideoData& video = VideoData::getInstance();
		//glOrtho(0, video.windowWidth, 0, video.windowHeight, -1, 1);
		glOrtho(0, 800, 0, 600, -1000, 1000);		//TODO: use actual window size

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			draw();		
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);*/
}