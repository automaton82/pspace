#ifndef _SUBSPACETEXTBOX_H_
#define _SUBSPACETEXTBOX_H_

#include "ExtendedTextBuffer.h"

class SubspaceTextBox :
	public ExtendedTextBuffer
{
public:
	SubspaceTextBox();
	virtual ~SubspaceTextBox();

	void clearHeader();
	double getDisplayHeight() const;
	double getDisplayWidthStart() const;		// left display just after the border
	double getHeaderDisplayHeight() const;
	double getLineDisplayHeight(Uint line) const;
	void setHeader(const string& header, Uchar color = 0);
	void setHeaderPadding(double headerPadTop, double headerPadBottom);
	void writeHeader(const string& text, Uchar color = 0);
			
	////////////////////////////

	virtual void draw() const;
	
protected:

	virtual void drawBackground() const;
	virtual void drawBorder() const;
	void drawHeader() const;
	void drawSeparator() const;

protected:

	double headerPadTop_, headerPadBottom_;
	double separatorWidth_;

	BasicTextBuffer header_;
};

#endif