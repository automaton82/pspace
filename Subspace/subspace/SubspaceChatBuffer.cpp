#include "SubspaceChatBuffer.h"

#include "AsciiUtil.h"
#include "SubspaceSettings.h"

const string SubspaceChatBuffer::whitespace = " \n\t";

SubspaceChatBuffer::SubspaceChatBuffer() : 
	lineWidth_(100),
	rebuildCache_(false)
{
	currentChunk_.colorIndex = COLOR_Blue;

	cacheNewline(0);
}

SubspaceChatBuffer::~SubspaceChatBuffer()
{
}

void SubspaceChatBuffer::setFont(const TextureFont& font)
{
	font_ = font;
}

void SubspaceChatBuffer::setHeader(const string& header, Uchar colorIndex)
{
	currentChunk_.header = header;
	currentChunk_.headerColor = colorIndex;
}

void SubspaceChatBuffer::setHeaderWidth(Uint width)
{
	headerWidth_ = width;
	rebuildCache_ = true;
}

void SubspaceChatBuffer::setLineWidth(Uint width)
{
	lineWidth_ = width;
	rebuildCache_ = true;
}

void SubspaceChatBuffer::setLinesDisplayed(Uint offset, Uint lines)
{
	linesDisplayOffset_ = offset;
	linesDisplayed_ = lines;
}

void SubspaceChatBuffer::flush()
{
	if(currentChunk_.text.size() > 0)
	{
		addChunk(currentChunk_);
		currentChunk_.text.clear();
	}

	if(rebuildCache_)
	{
		cache();
		rebuildCache_ = false;
	}
}

void SubspaceChatBuffer::put(char c)
{
	currentChunk_.text += c;
}

void SubspaceChatBuffer::write(const char* s, int size)
{
	currentChunk_.text.append(s, size);
}

void SubspaceChatBuffer::write(const string& s)
{
	currentChunk_.text += s;
}

void SubspaceChatBuffer::addMessage(const string& text, Uchar colorIndex)
{
	Chunk newChunk(text, colorIndex);

	if(currentLine_->size() > 0 && currentLine_->back().equalState(newChunk))
		currentLine_->back().text += newChunk.text;

	addChunk(newChunk);
}

void SubspaceChatBuffer::addMessage(const string& text, Uchar textColor, const string& header, Uchar headerColor)
{
	Chunk newChunk(text, textColor, header, headerColor);

	if(currentLine_->size() > 0 && currentLine_->back().equalState(newChunk))
		currentLine_->back().text += newChunk.text;

	addChunk(newChunk);
}

void SubspaceChatBuffer::display() const
{
	Uint width;

	Uint i;
	for(i = linesDisplayOffset_; i < linesDisplayOffset_+linesDisplayed_ && i < cachedLines_.size(); ++i)
	{
		width = 0;
		const ChunkList& line = cachedLines_[i];

		glTranslated(0, -(double)font_.getFontHeight(), 0);

		ChunkList::const_iterator j;
		for(j = line.begin(); j != line.end(); ++j)
		{
			const Chunk& c = (*j);
			font_.displayString(c.text.c_str(), c.colorIndex);
			width += c.text.size();

			glTranslated(c.text.size() * font_.getFontWidth(), 0, 0);
		}
		glTranslated(-(double)width * font_.getFontWidth(), 0, 0);
	}
}

void SubspaceChatBuffer::update(double time)
{
}

void SubspaceChatBuffer::addChunk(const Chunk& c)
{
	chunks_.push_back(c);
	cacheChunk(c);
}

void SubspaceChatBuffer::cache()
{
	cachedLines_.clear();
	    
	if(chunks_.size() == 0)
		return;

	cacheNewline(0);

	ChunkList::iterator i;
	for(i = chunks_.begin(); i != chunks_.end(); ++i)
	{
		Chunk& c = (*i);

		cacheChunk(c);
	}
}

void SubspaceChatBuffer::cacheChunk(const Chunk& c)
{
	string str = c.text;
	Uint offset = 0, lastOffset = 0;
	Chunk newChunk;

	if(str.size() == 0)	//empty chunk
		return;

	offset = str.find_first_of(whitespace, 0);	//parse into whitespace and words
	while(offset < str.size())	//grouping found
	{
		if(str[offset] == '\n')	//newline
		{
			newChunk = c;
			newChunk.text = str.substr(lastOffset, offset-lastOffset);
			cacheWord(newChunk);

			cacheNewline(0);
        }
		else //if(AsciiUtil::isWhiteSpace(str[offset]))	//whitespace, not '\n'
		{
			newChunk = c;
			newChunk.text = str.substr(lastOffset, offset-lastOffset+1);

			cacheWord(newChunk);
		}
		++offset;	//move past whitespace
	
		lastOffset = offset;
		offset = str.find_first_of(whitespace, offset);	
	}
	
	offset = min(str.size(), offset);	//check for bad offsets - if no whitespace is found

	if(offset-lastOffset > 0)	//leftover word
	{
		newChunk = c;
		newChunk.text = str.substr(lastOffset, offset-lastOffset);
		cacheWord(newChunk);	
	}
}

void SubspaceChatBuffer::cacheHeader(const Chunk& c)
{
	if(currentLine_->size() != 0)
		return;
	
	Chunk newChunk(c);
	newChunk.text.clear();
	currentLine_->push_back(newChunk);

	newChunk.text = AsciiUtil::fixedWidthString(newChunk.header, headerWidth_, AsciiUtil::leftJustified);
	cacheWord(newChunk);
}


void SubspaceChatBuffer::cacheNewline(const Chunk* headChunk)
{
	ChunkList newline;
	
	cachedLines_.push_back(newline);
	currentLine_ = &cachedLines_.back();

	if(headChunk && headChunk->header.size() > 0)
	{
		cacheHeader(*headChunk);
		/*Chunk newChunk(*headChunk);
		newChunk.text = newChunk.header;
		currentLine_->push_back(newChunk);*/
	}

	currentWidth_ = 0;
}

void SubspaceChatBuffer::cacheWord(const Chunk& c)
{
	string str = c.text;
	string head = c.header;
	Chunk newChunk;

	if(currentWidth_ + str.size() <= lineWidth_)	//word fits in current line
	{
		if(currentLine_->size() == 0)
		{
			cacheHeader(c);
			
			cacheWord(c);
		}
		else if(currentLine_->back().equalState(c))	//join chunks with the same options
		{
			currentLine_->back().text += str;
			currentWidth_ += str.size();
		}
		else	//put others on a new line
		{
			cacheNewline(&c);
			currentLine_->push_back(c);

			currentWidth_ += str.size();
		}
	}
	else	//word does not fit in current line
	{
		string trimmed = AsciiUtil::clipWhitespace(str, false, true);	//try without whitespace
		if(currentWidth_ + str.size() <= lineWidth_)
		{
			newChunk = c;
			newChunk.text = trimmed;
			cacheWord(newChunk);
		}
		else	//split up the word
		{
			newChunk = c;
			newChunk.text = c.text.substr(0, lineWidth_-currentWidth_);
			cacheWord(newChunk);	//first part

			cacheNewline(&c);	
			
			newChunk.text = c.text.substr(lineWidth_-currentWidth_);
			cacheWord(newChunk);	//rest
		}
	}
}
