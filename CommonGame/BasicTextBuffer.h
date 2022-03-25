//David Millman

#ifndef _BASICTEXTBUFFER_H_
#define _BASICTEXTBUFFER_H_

#include <list>
using std::list;

#include "TextureFont.h"

//intended to be write only, read/display is done internally
//NO WORD WRAP IN THIS

class BasicTextBuffer
{
public:

	typedef vector<Uchar> ColorList;
	struct Chunk
	{
		size_t size() const 
		{
			return text.size();
		}			

		string text;
		ColorList colors;
	};

	typedef vector<Chunk> ChunkLines;

public:

	BasicTextBuffer();
	virtual ~BasicTextBuffer();

	virtual void clear();

	virtual double getDisplayHeight() const;
	virtual double getDisplayWidth() const;
	const TextureFont& getFont() const;
	Uint getTabWidth() const;

	void setColor(Uchar color);
	void setFont(const TextureFont& font);
	void setTabWidth(Uint width);	//converts tabs to spaces - only affects tabs inserted after this is set
	virtual Uint size() const;	//number of lines

	//stream functions
	virtual void flush();					//could cache display?
	void put(char c);
	void put(char c, Uchar color);				//does not affect current color state
	void write(const string& text);
	void write(const string& text, Uchar color);

	////////////////////////////

	virtual void draw() const;

	//virtual void drawBackground() const;
	//virtual void drawBorder() const;
	virtual void drawText() const;

	virtual void update(double time);

protected:

	void writeChar(char c);		//routes to other functions
	void writeLetter(char c);
	void writeNewline();
	void writeTab();

protected:

	TextureFont font_;
	//double displayWidth_, displayHeight_;
	//double displayPosX_, displayPosY_;	//Y is distance from top
	Uint maxWidth_;

	//formatting
	Uint tabWidth_;

	//text data
	ChunkLines lines_;
	Chunk* currentLine_;
	Uchar currentColor_;
};

#endif