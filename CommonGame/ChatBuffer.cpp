#include "SubspaceChatBuffer.h"

#include "AsciiUtil.h"
#include "SubspaceSettings.h"

const string SubspaceChatBuffer::whitespace = " \n\t";

SubspaceChatBuffer::SubspaceChatBuffer() : 
	lineWidth_(100),
	rebuildCache_(false)
{
	currentChunk_.colorIndex = COLOR_Blue;
}

SubspaceChatBuffer::~SubspaceChatBuffer()
{
}

void SubspaceChatBuffer::setFont(const TextureFont& font)
{
	font_ = font;
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
	addChunk(currentChunk_);
	currentChunk_.text.clear();

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

	addChunk(newChunk);
}

void SubspaceChatBuffer::display() const
{
	Uint width;

	Uint i;
	for(i = linesDisplayOffset_; i < linesDisplayOffset_+linesDisplayed_ && i < cachedChunks_.size(); ++i)
	{
		width = 0;
		const ChunkList& line = cachedChunks_[i];

		glTranslated(0, -(double)font_.getFontHeight(), 0);

		ChunkList::const_iterator j;
		for(j = line.begin(); j != line.end(); ++j)
		{
			const Chunk& c = (*j);
			font_.displayString(c.text.c_str(), c.colorIndex);
			width += c.text.size();

			glTranslated(c.text.size() * font_.getFontWidth(), 0, 0);
		}
		glTranslated(-width * font_.getFontWidth(), 0, 0);
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
	cachedChunks_.clear();
	    
	if(chunks_.size() == 0)
		return;

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
	bool onWord = false;

	if(str.size() == 0)	//empty chunk
		return;

	offset = str.find_first_of(whitespace, 0);	//parse into whitespace and words
	while(offset < str.size())	//grouping found
	{
		if(str[offset] == '\n')	//newline
		{
			cacheWord(Chunk(str.substr(lastOffset, offset-lastOffset), c.colorIndex));

            cachedChunks_.push_back(ChunkList());
			currentLine_ = &cachedChunks_.back();

		}
		else //if(AsciiUtil::isWhiteSpace(str[offset]))	//whitespace, not '\n'
		{
			cacheWord(Chunk(str.substr(lastOffset, offset-lastOffset+1), c.colorIndex));
		}
		++offset;	//move past whitespace
	
		lastOffset = offset;
		offset = str.find_first_of(whitespace, offset);	
	}
		
	cacheWord(Chunk(str.substr(lastOffset, offset-lastOffset), c.colorIndex));	//leftover word
}


void SubspaceChatBuffer::cacheNewline()
{
	ChunkList newline;

	cachedChunks_.push_back(newline);
	currentLine_ = &cachedChunks_.back();

	currentWidth_ = 0;
}

void SubspaceChatBuffer::cacheWord(const Chunk& c)
{
	string str = c.text;

	if(currentWidth_ + str.size() <= lineWidth_)	//word fits in current line
	{
		if(currentLine_->back().equalState(c))	//join chunks with the same options
			currentLine_->back().text += str;
		else
			currentLine_->push_back(c);

		currentWidth_ += str.size();
	}
	else	//word does not fit in current line
	{
		string trimmed = AsciiUtil::clipWhitespace(str, false, true);	//try without whitespace
		if(currentWidth_ + str.size() <= lineWidth_)
		{
			cacheWord(Chunk(trimmed, c.colorIndex));
		}
		else	//split up the word
		{
			cacheWord(Chunk(c.text.substr(0, lineWidth_-currentWidth_), c.colorIndex));	//first part
			cacheNewline();
			cacheWord(Chunk(c.text.substr(lineWidth_-currentWidth_), c.colorIndex));
		}
	}
}
