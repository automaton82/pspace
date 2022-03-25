//David Millman

#ifndef _EXTENDEDTEXTBUFFER_H_
#define _EXTENDEDTEXTBUFFER_H_

#include "BasicTextBuffer.h"
#include "Color.h"

//intended to be write only, read/display is done internally

class ExtendedTextBuffer : 
	public BasicTextBuffer
{
public:

	ExtendedTextBuffer();
	virtual ~ExtendedTextBuffer();

	virtual double getDisplayHeight() const;
	virtual double getDisplayWidth() const;
	Uint getLineDisplayOffset() const;
	Uint getLinesDisplayed() const;
	
	void setBackgroundColor(const Color& c);
	void setBorderColor(const Color& c);
	void setLineDisplayOffset(Uint offset);					// start drawing at this line
	void setLinesDisplayed(Uint numLines);					// number of lines to draw
	//void setLinesDisplayed(Uint offset, Uint numLines);		// TODO: split this shit up
	void setLineWidth(Uint width);				//line width in characters
												//word wrap formatting is done during display - not cached
												//  for a small cost in draw time, 'caching' time is ~0
	void setPadding(double top, double bottom, double left, double right);
	virtual Uint size() const;		
	
	virtual void clear();
	virtual void flush();					//caches data - builds line indexes

	void print();
	////////////////////////////

	virtual void draw() const;
	virtual void drawText() const;

protected:
	
	void buildIndexes();	
	void buildIndexesFrom(Uint lineIndex, Uint offset);		//used to append indexes from new writes

	virtual void drawBackground() const;
	virtual void drawBorder() const;

	struct IndexData
	{
		IndexData(Uint i, Uint o) : lineIndex(i), offset(o)
		{}

		Uint lineIndex;
		Uint offset;
	};
	typedef vector<IndexData> IndexList;

protected:

	IndexList indexes_;				//line indexes, with word wrap

	//extended properties
	Color backgroundColor_;
	Color borderColor_;
	double borderWidth_;
	double padTop_, padBottom_, padLeft_, padRight_;
	Uint lineWidth_;
	Uint lineDisplayOffset_, linesDisplayed_;

	Uint lastOffset_, lastIndex_;
	//bool changed_;		//has the buffer changed? used for caching number of lines
};

#endif