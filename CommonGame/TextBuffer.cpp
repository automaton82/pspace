#include "TextBuffer.h"

#include "AsciiUtil.h"
using namespace AsciiUtil;

TextBuffer::TextBuffer()
{
}

TextBuffer::~TextBuffer()
{
}

void TextBuffer::clear()
{
	chunks_.clear();
	readPosition_ = chunks_.end();
}

Uint TextBuffer::size() const
{
	return (Uint)chunks_.size();
}

void TextBuffer::setDefaultFont(const TextureFont& font)
{
	defaultFont_ = font;
}

void TextBuffer::write(const string& text, const Color& tint, const TextureFont* font)
{
	Chunk chunk;
	vector<string> lines;
	size_t offset = 0;

	lines = tokenize(text, "\n");	//parse line endings
	
	vector<string>::iterator i;
	for(i = lines.begin(); i != lines.end(); ++i)
	{
		chunk.text = (*i);
		chunk.color = tint;

		if(!font)
			chunk.font = &defaultFont_;
		else
			chunk.font = font;

		chunks_.push_back(chunk);

		offset += (*i).length();	//length up to first '\n'
		if(offset < text.length())	//if not at the end, a '\n' is in between
		{
			offset += 1;
			endline();
		}
	}
}

void TextBuffer::writeln(const string& text, const Color& tint, const TextureFont* font)
{
	write(text + "\n", tint, font);
}

void TextBuffer::endline()
{
	Chunk c("", Color(1.0, 1.0, 1.0), &defaultFont_, true);

	chunks_.push_back(c);
}

void TextBuffer::initRead() const
{
	readPosition_ = chunks_.begin();
}

bool TextBuffer::readChunk(TextBuffer::Chunk* chunk) const
{
	bool retval = false;

	if(readPosition_ != chunks_.end())
	{
		*chunk = (*readPosition_);
		++readPosition_;

		retval = true;
	}

	return retval;
}