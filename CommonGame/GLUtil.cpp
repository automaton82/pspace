#include "GLUtil.h"

#include <assert.h>

#include "Vector.h"

namespace GLUtil
{

void drawUnitQuad()
{
	glBegin(GL_QUAD_STRIP);
		glVertex2d(1, 1);	
		glVertex2d(0, 1);
		glVertex2d(1, 0);
		glVertex2d(0, 0);
	glEnd();
}

void drawQuad(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUAD_STRIP);
		glVertex2d(x2, y2);	
		glVertex2d(x1, y2);
		glVertex2d(x2, y1);
		glVertex2d(x1, y1);
	glEnd();
}

void drawQuad(double width, double height)
{
	glBegin(GL_QUAD_STRIP);
		glVertex2d(width, height);	
		glVertex2d(0, height);
		glVertex2d(width, 0);
		glVertex2d(0, 0);
	glEnd();
}

void drawLineRect(double width, double height)
{
	glBegin(GL_LINE_LOOP);
		glVertex2d(width, height);
		glVertex2d(0, height);
		glVertex2d(0, 0);
		glVertex2d(width, 0);
		glVertex2d(width, height);
	glEnd();
}

void setProjectionMatrices()
{
	glGetDoublev(GL_MODELVIEW_MATRIX, modelviewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
	glGetIntegerv(GL_VIEWPORT, viewportMatrix);
}

void projectPoint(double x, double y, double z, double* wx, double* wy, double* wz)
{
	gluProject(x, y, z, modelviewMatrix, projectionMatrix, viewportMatrix, wx, wy, wz);
}
void unprojectPoint(double wx, double wy, double wz, double *x, double* y, double* z)
{
	gluUnProject(wx, wy, wz, modelviewMatrix, projectionMatrix, viewportMatrix, x, y, z);
}

void getViewVector(double wx, double wy, double* x0, double* y0, double* z0, double* xd, double* yd, double* zd)
{
	Vector nearPoint, farPoint;
	unprojectPoint(wx, wy, 0, &nearPoint.x_, &nearPoint.y_, &nearPoint.z_);
	unprojectPoint(wx, wy, 1, &farPoint.x_, &farPoint.y_, &farPoint.z_);

	Vector view = (farPoint - nearPoint).unitVector();
	
	*x0 = nearPoint.x_;
	*y0 = nearPoint.y_;
	*z0 = nearPoint.z_;

	*xd = view.x_;
	*yd = view.y_;
	*zd = view.z_;
}

void getViewVector(double wx, double wy, Vector* viewOrigin, Vector* viewDirection)
{
	getViewVector(wx, wy, &viewOrigin->x_, &viewOrigin->y_, &viewOrigin->z_, &viewDirection->x_, &viewDirection->y_, &viewDirection->z_);	
}


static double orthoX_, orthoY_, orthoWidth_, orthoHeight_, orthoNear_, orthoFar_;
void setOrthoMode(double x, double y, double width, double height, double nearClip, double farClip)
{
	orthoX_ = x;
	orthoY_ = y;
	orthoWidth_ = width;
	orthoHeight_ = height;
	orthoNear_ = nearClip;
	orthoFar_ = farClip;
}

void enterOrthoMode()
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

void exitOrthoMode()
{
	//enter ortho mode
		//draw
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

static GLdouble color[4];
void saveColor()
{
	glGetDoublev(GL_CURRENT_COLOR, color);
}

void restoreColor()
{
	glColor4d(color[0], color[1], color[2], color[3]);
}

}	//namespace GLUtil