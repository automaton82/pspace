 //David Millman

#include "TextBox.h"

TextBox::TextBox() :
	width_(1), height_(1), currentWidth_(0), currentHeight_(0), currentLine_(0)
{
}

TextBox::TextBox(Uint width, Uint height) : 
	width_(width), height_(height), currentWidth_(0), currentHeight_(0), currentLine_(0)
{
}

TextBox::~TextBox()
{
	TextLines::iterator i;
	for(i = text_.begin(); i != text_.end(); ++i)
	{
		delete (*i);
	}
}

void TextBox::setFont(const TextureFont& font)
{
	font_ = font;
}

void TextBox::setWidth(Uint width)
{
	width_ = width;
}

void TextBox::setHeight(Uint height)
{
	height_ = height;
}

Uint TextBox::size() const
{
	return currentHeight_;
}

void TextBox::setLineHeader(const string& header, int color)
{
    header_.text = header;
	header_.color = color;
}

void TextBox::addChar(char c, int color)
{
	string s;
	s += c;

	addWord(s, color);
}

void TextBox::addWord(const string& s, int color)
{
	TextBlock newText(s, color);

	if(s.length() == 0)	//no word
		return;

	if(text_.size() == 0)
		newLine();

	int headerWidth = (int)header_.text.length();
	if(s.length()+headerWidth > width_)		//check for a multiple line word
	{
		for(unsigned int i=0; i < s.length(); i += width_-headerWidth)	//recursively add pieces
			addWord(s.substr(i, width_-headerWidth), color);
	}
	else
	{
		if(currentWidth_ + s.length() > width_-headerWidth)
			newLine();

		currentLine_->push_back(newText);
	}

	currentWidth_ += (Uint)s.length();
}

void TextBox::addLine(const string& s, int color, bool parseWhiteSpace)
{
	newLine();

	if(!parseWhiteSpace)
	{
		addWord(s, color);
	}
	else
	{
		TextBlock newText(s, color);
		string word;
		bool onWhiteSpace = false;

		string::const_iterator i;
		for(i = s.begin(); i != s.end(); ++i)
		{
			char c = *i;

			if(AsciiUtil::isEndOfLine(c))
				newLine();
			else if(AsciiUtil::isWhiteSpace(c))
			{
				if(onWhiteSpace)
					word += c;
				else
				{
					addWord(word, color);	//add previous word

					word.clear();
					word += c;
				}

				onWhiteSpace = true;
			}
			else
			{
				if(!onWhiteSpace)
					word += c;
				else
				{
					addWord(word, color);	//add previous whitespace

					word.clear();
					word += c;
				}
	
				onWhiteSpace = false;
			}
		}

		addWord(word, color);	//leftover word
	}
}

void TextBox::newLine()
{
	TextBlockList* line = new TextBlockList;

	text_.push_back(line);
	currentLine_ = text_.back();

	currentWidth_ = 0;
	++currentHeight_;

	while(currentHeight_ > height_)
	{
		TextBlockList* temp = text_.front();
        text_.pop_front();
		delete temp;

		--currentHeight_;
	}

	if(header_.text.length() > 0)
	{
		assert(header_.text.length() <= width_);

		currentLine_->push_back(header_);
	}
}

void TextBox::clear()
{
	TextLines::iterator i;
	for(i = text_.begin(); i != text_.end(); ++i)
	{
		delete (*i);
		i = text_.erase(i);
	}
}

void TextBox::draw() const
{
	TextLines::const_reverse_iterator i;
	unsigned int j;

	glPushMatrix();

	glTranslated(0, (double) (-(int)currentHeight_ * font_.getFontHeight()), 0);	//translate to bottom of text lines

	for(i=text_.rbegin(), j=0; i != text_.rend(); ++i, ++j)			//start at the last line
	{
		const TextBlockList& line = *(*i);
		drawLine(line);
		
		glTranslated(0, (double)font_.getFontHeight(), 0);			//translate up
	}

	glPopMatrix();	
}

void TextBox::drawLine(const TextBlockList& line) const
{
	glPushMatrix();

	TextBlockList::const_iterator i;
	for(i = line.begin(); i != line.end(); ++i)
	{
		const TextBlock& word = (*i);
		font_.displayString(word.text.c_str(), word.color);

		glTranslated( (double)(word.text.length() * font_.getFrameWidth()), 0, 0);
	}

	glPopMatrix();
}

