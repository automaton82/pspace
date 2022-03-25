#ifndef _WINDOWMANAGER_H_
#define _WINDOWMANAGER_H_

#include <list>
using std::list;

#include "InputEvent.h"
#include "OrthographicObject.h"
#include "WindowObject.h"

class WindowManager : 
	public OrthographicObject
{
private:

	struct WindowData
	{
		WindowData() : open(false), window(0), x(0), y(0)
		{}

		bool open;
		bool locked;	//TODO: implement this - no resize, no move, no close
		WindowObject* window;
		double x, y;
	};

	typedef list<WindowData> WindowList;

public:

	enum ResizeType
	{
		RESIZE_UNKNOWN	= 0x00,
		RESIZE_TOP		= 0x01,
		RESIZE_LEFT		= 0x02,
		RESIZE_BOTTOM	= 0x04,
		RESIZE_RIGHT	= 0x08,
		RESIZE_ALL		= 0x0F,
	};

public:
	WindowManager();
	~WindowManager();

	
	bool handleDragInput(InputEvent input, double x0, double y0, double x1, double y1);
	bool handleInput(InputEvent input, double x, double y);

	//TODO: move object position, resizing, open/close to window object itself, selection still done by manager

	bool addWindow(WindowObject* window, double x=0, double y=0);
	bool closeWindow(WindowObject* window);
	WindowObject* getSelectedWindow() const;
	bool moveWindow(WindowObject* window, double x, double y);	//moves BY x,y
	bool moveWindowTo(WindowObject* window, double x, double y);	//moves TO x,y
	bool openWindow(WindowObject* window);
	bool resizeWindow(WindowObject* window, double width, double height);
	bool selectWindow(WindowObject* window);	//moves window to top

	void draw() const;	//takes care of window ordering
	void update(double time);

private:

	WindowList::iterator findWindow(WindowObject* window);
	bool isInWindowBounds(const WindowData& windowData, double x, double y) const;
	int getInResizeArea(const WindowData& windowData, double x, double y) const;
	void moveWindow(WindowData& windowData, double xChange, double yChange);
	void resizeWindow(WindowData& windowData, double xChange, double yChange, int resizeType);
	void resizeWindowTo(WindowData& windowData, double width, double height, int resizeType);
	void selectWindowIter(WindowList::iterator i);

private:

	WindowList windows_;
	WindowObject* selectedWindow_;

	double resizeBorderWidth_;		
	int resizeType_;
};

#endif