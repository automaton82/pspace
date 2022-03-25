//David Millman

#ifndef _TEXTWINDOW_H_
#define _TEXTWINDOW_H_

#include <list>
using std::list;

#include "ExtendedWindowObject.h"
#include "TextBuffer.h"

class TextWindow :
	public ExtendedWindowObject
{
public:
	TextWindow();
	virtual ~TextWindow();

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