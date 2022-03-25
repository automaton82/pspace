#include "BasicTextBuffer.h"

#include "AsciiUtil.h"

BasicTextBuffer::BasicTextBuffer() : 
	currentColor_(0),
	currentLine_(0),
	maxWidth_(0),
	tabWidth_(4)
{
}

BasicTextBuffer::~BasicTextBuffer()
{
}

double BasicTextBuffer::getDisplayHeight() const
{
	return (font_.getFontHeight() * size());
}

double BasicTextBuffer::getDisplayWidth() const
{
	return (font_.getFontWidth() * maxWidth_);
}

const TextureFont& BasicTextBuffer::getFont() const
{
	return font_;
}

Uint BasicTextBuffer::getTabWidth() const
{
	return tabWidth_;
}

void BasicTextBuffer::setColor(Uchar color)
{
	currentColor_ = color;
}

void BasicTextBuffer::setFont(const TextureFont& font)
{
	font_ = font;
}

void BasicTextBuffer::setTabWidth(Uint width)
{
	tabWidth_ = max(1, width);
}

Uint BasicTextBuffer::size() const
{
	Uint lines = (Uint)lines_.size();
	if(lines_.size() > 0 && lines_.back().size() == 0)
		--lines;

	return lines;
}

void BasicTextBuffer::clear()
{
	lines_.clear();
	currentLine_ = 0;
}

void BasicTextBuffer::flush()
{
}

void BasicTextBuffer::put(char c)
{
	writeChar(c);
}

void BasicTextBuffer::put(char c, Uchar color)
{
	Uchar temp = currentColor_;
	currentColor_ = color;		//save color

	put(c);

	currentColor_ = temp;		//restore color
}

void BasicTextBuffer::write(const string& text)
{
	string::const_iterator i;
	for(i = text.begin(); i != text.end(); ++i)
		writeChar(*i);
}

void BasicTextBuffer::write(const string& text, Uchar color)
{
	Uchar temp = currentColor_;
	currentColor_ = color;			//save color

	write(text);

	currentColor_ = temp;		//restore color
}

void BasicTextBuffer::draw() const
{
	drawText();
}

void BasicTextBuffer::drawText() const
{
	ChunkLines::const_iterator i;
	string::const_iterator s;
	ColorList::const_iterator c;

	glTranslated(0, font_.getFontHeight() * size(), 0);	//move to top of buffer

	for(i = lines_.begin(); i != lines_.end(); ++i)
	{
		const Chunk& line = (*i);
		
		glTranslated(0, -(double)font_.getFontHeight(), 0);	//move down to line
		for(s = line.text.begin(), c = line.colors.begin(); s != line.text.end(); ++s, ++c)
		{
			font_.displayChar(*s, *c);
			glTranslated(font_.getFontWidth(), 0, 0);		//move right by 1 char
		}
		glTranslated(-(double)line.size() * font_.getFontWidth(), 0, 0);	//move to beginning of line
	}
}

void BasicTextBuffer::update(double time)
{
}

void BasicTextBuffer::writeChar(char c)
{
	switch(c)
	{
	case '\t':
		writeTab();
		break;
	case '\n':
		writeNewline();
		break;
	default:
		writeLetter(c);
	}
}

void BasicTextBuffer::writeLetter(char c)
{
	if(!currentLine_)
		writeNewline();

	currentLine_->text += c;
	currentLine_->colors.push_back(currentColor_);

	if(currentLine_->size() > maxWidth_)
		maxWidth_ = (Uint)currentLine_->size();
}

void BasicTextBuffer::writeNewline()
{
	Chunk newline;
	
	lines_.push_back(newline);
	currentLine_ = &lines_.back();
}

void BasicTextBuffer::writeTab()
{
	if(!currentLine_)
		writeNewline();

	Uint numSpaces = (Uint)currentLine_->size() % tabWidth_;	//leftover spaces
	if(numSpaces == 0)			//full tab width
		numSpaces = tabWidth_;

	for(Uint i=0; i < numSpaces; ++i)
		writeLetter(' ');
}