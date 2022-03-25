//David Millman

#ifndef _SCROLLBARWINDOW_H_
#define _SCROLLBARWINDOW_H_

#include <list>
using std::list;

#include "ExtendedWindowObject.h"

class ScrollBarWindow :
	public ExtendedWindowObject
{
public:
	ScrollBarWindow();
	virtual ~ScrollBarWindow();

	void 

	void setTextBuffer(TextBuffer* buffer);
	void setWordWrap(bool b);

	virtual void onResize();

	virtual void draw() const;	
	void drawBuffer() const;
	virtual void update(double time);	//for animated text and such

protected:

	void cacheBuffer();

	typedef vector<TextBuffer> BufferList;

protected:

	TextBuffer* textBuffer_;

	bool wordWrap_ ;
	BufferList bufferCache_;	//each buffer represents one line
};

#endif