//David Millman

#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include "DataTypes.h"

#include "AsciiUtil.h"
#include "GameObject.h"
#include "TextureFont.h"

//TODO: make general text box - not just subspace

class TextBox : 
	public GameObject
{
public:
	TextBox();
	TextBox(Uint width, Uint height);
	virtual ~TextBox();

	//settings
	void setFont(const TextureFont& font);

	void setWidth(Uint width);			//does not reformat anything added before this
	void setHeight(Uint height);
	Uint size() const;						//current number of lines

	void setLineHeader(const string& header, int color);	//e.g.  [header]  line
	
	//add text	//note: color corresponds to a font set color (in the bmp)
	virtual void addChar(char c, int color);
	virtual void addWord(const string& s, int color);		

	virtual void addLine(const string& s, int color, bool parseWords = true);	//automatically adds at new line, parses words based on whitespace

	virtual void newLine();									//force jump to the next line

	//clear text
	void clear();

	//display
	void draw() const;

protected:
	
	struct TextBlock			//TODO: use color indexes for color (i.e. per letter)?
	{
		TextBlock() : color(0)
		{}

		TextBlock(const string& t, int c) : text(t), color(c)
		{}

		string text;
		int color;
	};

	typedef list<TextBlock> TextBlockList;
	typedef list<TextBlockList*> TextLines;

protected:

	void drawLine(const TextBlockList& line) const;
	
	TextLines text_;
	TextBlockList* currentLine_;

	Uint width_, height_;
	Uint currentWidth_, currentHeight_;		//for adding new words and lines

	TextBlock header_;

	TextureFont font_;
};

#endif