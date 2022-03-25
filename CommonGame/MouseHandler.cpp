//David Millman
//Computer Graphics
//Lights & Motion

#include "MouseHandler.h"

MouseHandler::MouseHandler() : mouseState_(0)
{
}

void MouseHandler::mouseClick(int button, int state, int x, int y)
{
	if(state==GLUT_DOWN)
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON: 
			mouseState_ |= (int)pow(2,GLUT_LEFT_BUTTON);
			break;
		case GLUT_MIDDLE_BUTTON:
			mouseState_ |= (int)pow(2,GLUT_MIDDLE_BUTTON);
			break;
		case GLUT_RIGHT_BUTTON:
			mouseState_ |= (int)pow(2,GLUT_RIGHT_BUTTON);
			break;
		}
	}
	if(state==GLUT_UP)
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON: 
			mouseState_ &= ~(int)pow(2,GLUT_LEFT_BUTTON);
			break;
		case GLUT_MIDDLE_BUTTON:
			mouseState_ &= ~(int)pow(2,GLUT_MIDDLE_BUTTON);
			break;
		case GLUT_RIGHT_BUTTON:
			mouseState_ &= ~(int)pow(2,GLUT_RIGHT_BUTTON);
			break;
		}
	}    
	//printf("button state: %d\n", mouseState_);
}

void MouseHandler::mouseMove(int x, int y)
{
	prevMousePos_ = mousePos_;	
	mousePos_.x_ = x;
	mousePos_.y_ = y;
}

int MouseHandler::getButtonState(int button) const
{
	return (mouseState_ & (int)pow(2,button));
}

int MouseHandler::getMouseState() const
{
	return mouseState_;
}

Point MouseHandler::getMousePos() const
{
	return mousePos_;
}

Point MouseHandler::getMouseChange() const
{
	//printf("Change: (%d,%d,%d)\n", mousePos_.x_-prevMousePos_.x_, mousePos_.y_-prevMousePos_.y_, mousePos_.z_-prevMousePos_.z_);
	return Point(mousePos_.x_-prevMousePos_.x_, mousePos_.y_-prevMousePos_.y_, mousePos_.z_-prevMousePos_.z_);
}