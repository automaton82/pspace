#include "ExtendedTextBuffer.h"

#include <iostream>

#include "AsciiUtil.h"

ExtendedTextBuffer::ExtendedTextBuffer() : 
	//changed_(true),
	backgroundColor_(10.0/255.0, 25.0/255.0, 10.0/255.0, 0.8),
	borderColor_(0.5, 0.5, 0.5, 1.0),
	borderWidth_(2.0),
	padTop_(1.0), padBottom_(1.0),  padLeft_(1.0), padRight_(1.0),

	lineWidth_(-1),
	lastIndex_(0),
	lastOffset_(0),
	lineDisplayOffset_(0),
	linesDisplayed_(-1)
{
}

ExtendedTextBuffer::~ExtendedTextBuffer()
{
}

double ExtendedTextBuffer::getDisplayHeight() const
{
	return (font_.getFontHeight() * min(linesDisplayed_, size()) + padTop_+padBottom_+borderWidth_);
}

double ExtendedTextBuffer::getDisplayWidth() const
{
	return (font_.getFontWidth() * min(lineWidth_, maxWidth_) + padLeft_+padRight_+borderWidth_);
}

Uint ExtendedTextBuffer::getLineDisplayOffset() const
{
	return lineDisplayOffset_;
}

Uint ExtendedTextBuffer::getLinesDisplayed() const
{
	return linesDisplayed_;
}

void ExtendedTextBuffer::setBackgroundColor(const Color& c)
{
	backgroundColor_ = c;
}

void ExtendedTextBuffer::setBorderColor(const Color& c)
{
	borderColor_ = c;
}

/*void ExtendedTextBuffer::setLinesDisplayed(Uint offset, Uint numLines)
{
	lineDisplayOffset_ = offset;
	linesDisplayed_ = numLines;
}*/

void ExtendedTextBuffer::setLineDisplayOffset(Uint offset)
{
	lineDisplayOffset_ = offset;
}

void ExtendedTextBuffer::setLinesDisplayed(Uint numLines)
{
	linesDisplayed_ = numLines;
}

void ExtendedTextBuffer::setLineWidth(Uint width)
{
	lineWidth_ = width;
	//changed_ = true;

	buildIndexes();
}

void ExtendedTextBuffer::setPadding(double top, double bottom, double left, double right)
{
	padTop_ = top;
	padBottom_ = bottom;
	padLeft_ = left;
	padRight_ = right;
}

Uint ExtendedTextBuffer::size() const
{
	return (Uint)indexes_.size();
}

void ExtendedTextBuffer::clear()
{
	BasicTextBuffer::clear();
	indexes_.clear();

	lastIndex_ = 0;
	lastOffset_ = 0;
}

void ExtendedTextBuffer::flush() 
{
	buildIndexesFrom(lastIndex_, lastOffset_);
}

void ExtendedTextBuffer::print()
{
	IndexList::const_iterator i;
	Uint nextLineOffset;
	const Chunk* line;

	for(i = indexes_.begin(); i != indexes_.end(); ++i)
	{
		const IndexData& data = (*i);
		line = &lines_[data.lineIndex];

		nextLineOffset = line->text.size();	//set next line

		++i;
		if(i != indexes_.end())		//next line index
		{
			Uint nextIndex = (*i).lineIndex;
			if(data.lineIndex == (*i).lineIndex)	//same line index
				nextLineOffset = (*i).offset;
		}
		--i;
		
		std::cout << line->text.substr(data.offset, nextLineOffset-data.offset) << std::endl;
	}
}

void ExtendedTextBuffer::draw() const
{
	glDisable(GL_TEXTURE_2D);
	drawBackground();

	glColor4d(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslated(padLeft_ + borderWidth_/2.0, padBottom_ + borderWidth_/2.0, 0);
		drawText();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	drawBorder();
}

void ExtendedTextBuffer::drawText() const
{
	IndexList::const_iterator i;
	string::const_iterator s, nextLineIter;
	ColorList::const_iterator c;
	const Chunk* line;
	Uint nextLineOffset = 0;

	Uint displayLines = min(linesDisplayed_, size());
	Uint lineNum = 0;

	glTranslated(0, font_.getFontHeight() * displayLines, 0);	//move to top of buffer
	for(i = indexes_.begin()+lineDisplayOffset_; i != indexes_.end() && lineNum < displayLines; ++i, ++lineNum)
	{
		const IndexData& data = (*i);
		line = &lines_[data.lineIndex];

		nextLineOffset = line->text.size();	//set next line

		++i;
		if(i != indexes_.end())		//next line index
		{
			if(data.lineIndex == (*i).lineIndex)	//same line index
				nextLineOffset = (*i).offset;
		}
		--i;

		nextLineIter = line->text.begin()+nextLineOffset;

		glTranslated(0, -(double)font_.getFontHeight(), 0);	//move down to line
		for(s = line->text.begin()+data.offset, c = line->colors.begin(); s != line->text.end() && s != nextLineIter; ++s, ++c)
		{
			font_.displayChar(*s, *c);
			glTranslated(font_.getFontWidth(), 0, 0);		//move right by 1 char
		}
		glTranslated(-(double) (nextLineOffset-data.offset) * font_.getFontWidth(), 0, 0);	//move to beginning of line
	}
}

void ExtendedTextBuffer::buildIndexes()
{
	indexes_.clear();

	buildIndexesFrom(0, 0);
}

void ExtendedTextBuffer::buildIndexesFrom(Uint lineIndex, Uint off)
{
	Uint offset = off, wordOffset;
	Uint currentWidth = off;

	Uint index = lineIndex;
	bool onWord = true;

	ChunkLines::iterator i;
	for(i = lines_.begin()+lineIndex; i != lines_.end(); ++i)
	{
		Chunk& line = (*i);

		if(index != lineIndex || indexes_.size()==0)		//add line if not the starting point, or no lines exist
		{
			//std::cout << "[" << index << ", " << offset << "]" << std::endl;
			indexes_.push_back(IndexData(index, 0));			
			offset = 0;
			currentWidth = 0;
			onWord = true;
		}
		
		if(line.size() > lineWidth_)	//line is larger than the line width
		{								//parse into words
			while(offset < line.size())
			{
				if(onWord)
					wordOffset = line.text.find_first_of(AsciiUtil::Whitespace, offset);		//get next word
				else
					wordOffset = line.text.find_first_not_of(AsciiUtil::Whitespace, offset);	//get next whitespace word
				if(wordOffset == line.text.npos)
					wordOffset = line.size();

				if(currentWidth + wordOffset-offset > lineWidth_)	//word doesn't fit on current line
				{
					if(wordOffset-offset > lineWidth_)				//word is too big, split up word
					{
						//std::cout << "[" << index << ", " << offset << "]" << std::endl;
						indexes_.push_back(IndexData(index, offset));	//index new line
						currentWidth = 0;	

						//std::cout << line.text.substr(offset, lineWidth_);
						offset += lineWidth_;		//piece of word
						currentWidth += lineWidth_;

						//std::cout << "[" << index << ", " << offset << "]" << std::endl;
						indexes_.push_back(IndexData(index, offset));	//index new line
						currentWidth = 0;	
					}
					else										//word will fit on one line
					{
						//std::cout << "[" << index << ", " << offset << "]" << std::endl;
						indexes_.push_back(IndexData(index, offset));	//index new line
						currentWidth = 0;

						//std::cout << line.text.substr(offset, wordOffset-offset);
						currentWidth += wordOffset-offset;
						offset = wordOffset;

						onWord = !onWord;
					}
				}
				else										//word fits, move on
				{
					//std::cout << line.text.substr(offset, wordOffset-offset);

					currentWidth += wordOffset-offset;
					offset = wordOffset;	

					onWord = !onWord;					
				}
			}
		}

		++index;
		/*else
		{
			std::cout << line.text << std::endl;
		}*/			
		//line fits continue on loop		
	}

	if(indexes_.size() == 0)
		lastIndex_ = 0;
	else
		lastIndex_ = indexes_.back().lineIndex;		
	lastOffset_ = offset;
}

void ExtendedTextBuffer::drawBackground() const
{
	double drawWidth = min(lineWidth_, maxWidth_) * font_.getFrameWidth();
	double drawHeight =  min(linesDisplayed_, size()) * font_.getFrameHeight();

	glColor4d(backgroundColor_.r, backgroundColor_.g, backgroundColor_.b, backgroundColor_.a);
	glBegin(GL_QUADS);			//background
		glVertex3d(drawWidth + padLeft_+padRight_ + borderWidth_/2.0, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, drawHeight + padTop_+padBottom_ + borderWidth_/2.0, 0);
		glVertex3d(drawWidth + padLeft_+padRight_ + borderWidth_/2.0, drawHeight + padTop_+padBottom_ + borderWidth_/2.0, 0);
	glEnd();
}


void ExtendedTextBuffer::drawBorder() const
{
	double drawWidth = min(lineWidth_, maxWidth_) * font_.getFrameWidth();
	double drawHeight = min(linesDisplayed_, size()) * font_.getFrameHeight();

	glColor4d(borderColor_.r, borderColor_.g, borderColor_.b, borderColor_.a);
	glLineWidth(borderWidth_);
	glBegin(GL_LINE_LOOP);			//main border
		glVertex3d(0, 0, 0);
		glVertex3d(drawWidth + padLeft_+padRight_ + borderWidth_, 0, 0);
		glVertex3d(drawWidth + padLeft_+padRight_ + borderWidth_, drawHeight + padTop_+padBottom_ + borderWidth_, 0);
		glVertex3d(0, drawHeight + padTop_+padBottom_ + borderWidth_, 0);
		glVertex3d(0, 0, 0);
	glEnd();
}