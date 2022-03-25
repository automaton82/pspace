#include "WindowManager.h"

#include <assert.h>
#include "GLUtil.h"
#include "TextureAnimated.h"

WindowManager::WindowManager() :
	selectedWindow_(0),
	resizeBorderWidth_(5.0),
	resizeType_(0)
{
}

WindowManager::~WindowManager()
{
}

bool WindowManager::handleDragInput(InputEvent input, double x0, double y0, double x1, double y1)
{
	bool retval = false;
	bool handled = false;

	//drag is only valid for selected window

	WindowData& wd = windows_.front();

	if(wd.open)
	{
		if(isInWindowBounds(wd, x0, y0))
		{
			handled = wd.window->handleDragInput(input, x0, y0, x1, y1);

			if(!handled)	//if the window doesn't handle the drag, move the window
				moveWindow(wd, x1-x0, y1-y0);
		}
		else
		{
			if(!resizeType_)
				resizeType_ = getInResizeArea(wd, x0, y0);

			if(resizeType_)		//drag window
			{
				double xChange = x1-x0;
				if(x0 < wd.x)	xChange = 0;
				double yChange = y1-y0;
				if(y0 < wd.y)	yChange = 0;

				resizeWindow(wd, xChange, yChange, resizeType_);
				//resizeWindowTo(wd, x1-wd.x, y1-wd.y, resizeType_);
			}
		}
	}

	return retval;
	
	/*if(input.isUnactivated())
	{
		resizeType = 0;
		return true;
	}

	WindowList::iterator i;
	for(i = windows_.begin(); i != windows_.end(); ++i)
	{
		WindowData& wd = (*i);

		if(wd.open)
		{
			if(isInWindowBounds(wd, x0, y0))
			{
				

				handled = wd.window->handleDragInput(input, x0, y0, x1, y1);

				if(!handled && wd.window == selectedWindow_)	//if the window doesn't handle the drag, move the window
					moveWindow(wd, x1-x0, y1-y0);
				break;
			}
			else
			{
				if(wd.window == selectedWindow_)
				{
					if(!resizeType)
						resizeType = getInResizeArea(wd, x0, y0);
				}
				else
				{
					resizeType = 0;
				}

				if(resizeType)		//drag window
				{
					resizeWindow(wd, x1-x0, y1-y0, resizeType);
					break;
				}
			}
		}
	}*/

	return retval;
}

bool WindowManager::handleInput(InputEvent input, double x, double y)
{
	bool retval = false;
	bool handled = false;

	selectedWindow_ = 0;
	resizeType_ = 0;

	WindowList::iterator i;
	for(i = windows_.begin(); i != windows_.end(); ++i)
	{
		WindowData& wd = (*i);

		if(wd.open && isInWindowBounds(wd, x, y))
		{
			handled = wd.window->handleInput(input, x - wd.x, y - wd.y);	//convert to window coordinates
			if(handled)
			{
				selectWindowIter(i);
				break;
			}
		}
	}

	return retval;
}

bool WindowManager::addWindow(WindowObject* window, double x, double y)
{
	assert(window);

	WindowData wd;
	wd.open = false;
	wd.window = window;
	wd.x = x;
	wd.y = y;

	windows_.push_back(wd);

	return true;
}

bool WindowManager::closeWindow(WindowObject* window)
{
	bool retval = false;
	WindowList::iterator i = findWindow(window);

	if(i != windows_.end())
	{
		(*i).open = false;
		retval = true;
	}

	return retval;
}


WindowObject* WindowManager::getSelectedWindow() const
{
	WindowObject* retval = 0;

	if(windows_.size() > 0)
	{
		const WindowData& wd = windows_.front();
		retval = wd.window;
	}
	return retval;
}

bool WindowManager::moveWindow(WindowObject* window, double x, double y)
{
	bool retval = false;
	WindowList::iterator i = findWindow(window);

	if(i != windows_.end())
	{
		WindowData& wd = (*i);
		
		wd.x += x;
		wd.y += y;
	}

	return retval;
}

bool WindowManager::moveWindowTo(WindowObject* window, double x, double y)
{
	bool retval = false;
	WindowList::iterator i = findWindow(window);

	if(i != windows_.end())
	{
		WindowData& wd = (*i);
		//retval = wd.window->move(x, y);

		wd.x = x;
		wd.y = y;
	}

	return retval;
}

bool WindowManager::openWindow(WindowObject* window)
{
	bool retval = false;
	WindowList::iterator i = findWindow(window);

	if(i != windows_.end())
	{
		(*i).open = true;
		retval = true;
	}

	return retval;
}

bool WindowManager::resizeWindow(WindowObject* window, double width, double height)
{
	//TODO: adjust for position

	WindowList::iterator i = findWindow(window);
	if(i != windows_.end())
	{
		WindowData& wd = (*i);

		//wd.window->resize(width, height);
		resizeWindow(wd, width - wd.window->getWidth(), height - wd.window->getHeight(), RESIZE_TOP | RESIZE_RIGHT);
	}

	return false;
}

bool WindowManager::selectWindow(WindowObject* window)
{
	assert(window);

	bool retval = false;
	WindowList::iterator i = findWindow(window);

	if(i != windows_.end())
	{
        selectWindowIter(i);
		retval = true;
	}

	/*if(i != windows_.end())
	{
		windows_.push_front(*i);	//move object to front
		windows_.erase(i);			//erase old object
		retval = true;

		selectedWindow_ = window;
	}*/

	return retval;
}

WindowManager::WindowList::iterator WindowManager::findWindow(WindowObject* window)
{
	//TODO: replace with std::find?

	WindowList::iterator i;

	for(i = windows_.begin(); i != windows_.end(); ++i)
	{
		WindowData& wd = (*i);

		if(wd.window == window)
			break;
	}

	return i;
}

bool WindowManager::isInWindowBounds(const WindowData& wd, double x, double y) const
{
	bool retval = false;

	double left, right, top, bottom;
	left = wd.x;
	right = left + wd.window->getWidth();
	bottom = wd.y;
	top = bottom + wd.window->getHeight();

	if(x >= left && x < right &&		//check if window in range
		y >= bottom && y < top)
	{
		retval = true;
	}

	return retval;
}

int WindowManager::getInResizeArea(const WindowData& wd, double x, double y) const
{
	int retval = RESIZE_UNKNOWN;

	double left, right, top, bottom;
	left = wd.x;
	right = left + wd.window->getWidth();
	bottom = wd.y;
	top = bottom + wd.window->getHeight();

	if(x >= left - resizeBorderWidth_ && x <= right + resizeBorderWidth_ &&
		y >= bottom - resizeBorderWidth_ && y <= top + resizeBorderWidth_)
	{
		if(x >= left - resizeBorderWidth_ && x <= left)	//left side
			retval |= RESIZE_LEFT;	
		else if(x >= right && x <= right + resizeBorderWidth_)	//right side
			retval |= RESIZE_RIGHT;

		if(y >= bottom - resizeBorderWidth_ && y <= bottom)
			retval |= RESIZE_BOTTOM;
		else if(y >= top && y <= top + resizeBorderWidth_)
			retval |= RESIZE_TOP;
	}

	return retval;
}

void WindowManager::moveWindow(WindowData& windowData, double xChange, double yChange)
{
	windowData.x += xChange;
	windowData.y += yChange;
}

void WindowManager::resizeWindow(WindowData& windowData, double xChange, double yChange, int resize)
{
	double newWidth = windowData.window->getWidth();
	double newHeight = windowData.window->getHeight();

	if(resize & RESIZE_RIGHT && newWidth+xChange < 1)
		xChange = fmod(xChange, 1);
	else if(resize & RESIZE_LEFT && newWidth-xChange < 1)
		xChange = fmod(xChange, 1);

	if(resize & RESIZE_TOP && newHeight+yChange < 1)
		yChange = fmod(yChange, 1);
	else if(resize & RESIZE_BOTTOM && newHeight-yChange < 1)
		yChange = fmod(yChange, 1);

	
	if(resize & RESIZE_RIGHT)
		newWidth += xChange;
	if(resize & RESIZE_LEFT)
		newWidth -= xChange;		//negative xChange if increasing size

	if(resize & RESIZE_TOP)	
		newHeight += yChange;
	if(resize & RESIZE_BOTTOM)
		newHeight -= yChange;

	if(windowData.window->resize(newWidth, newHeight))	
	{									//if it resizes in these cases, move the window
		if(resize & RESIZE_LEFT && newWidth > 0)
			windowData.x += xChange;
		if(resize & RESIZE_BOTTOM && newHeight > 0)
			windowData.y += yChange;
	}
}

void WindowManager::resizeWindowTo(WindowData& wd, double width, double height, int resize)
{
	double newWidth = wd.window->getWidth(); 
	double newHeight = wd.window->getHeight();
	double adjustX = 0, adjustY = 0;

	if(resize & RESIZE_LEFT || resize & RESIZE_RIGHT)
		newWidth = width;
	if(resize & RESIZE_TOP || resize & RESIZE_BOTTOM)
		newHeight = height;

	if(resize & RESIZE_LEFT)
		adjustX = width - wd.x;
	if(resize & RESIZE_BOTTOM)
		adjustY = height - wd.y;

	if(wd.window->resize(newWidth, newHeight))	
	{
		wd.x += adjustX;
		wd.y += adjustY;
	}
}

void WindowManager::selectWindowIter(WindowList::iterator i)
{
	if(i != windows_.end())
	{
		windows_.push_front(*i);
		windows_.erase(i);

		selectedWindow_ = windows_.front().window;

		selectedWindow_->onSelect();
	}
}

void WindowManager::draw() const
{
	WindowList::const_reverse_iterator i;
	for(i = windows_.rbegin(); i != windows_.rend(); ++i)		//draw back to front
	{
		const WindowData& wd = (*i);

		glPushMatrix();
			glTranslated(wd.x, wd.y, 0);
			wd.window->draw();
		glPopMatrix();

		//wd.window->display();
	}

	/*TextureAnimated test;
	test.load("../../Images/raster/pngtest2.png");
	test.display();*/

    //glColor4d(1.0, 0, 0, 1.0);
	//GLUtil::drawQuad(30, 30);
}

void WindowManager::update(double time)
{
	WindowList::iterator i;
	for(i = windows_.begin(); i != windows_.end(); ++i)
	{
		WindowData& wd = (*i);

		wd.window->update(time);
	}
}