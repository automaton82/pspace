#include "SubspaceHelp.h"

#include <fstream>
#include "SubspaceSettings.h"

SubspaceHelp::SubspaceHelp() : 
	currentColor_(COLOR_Gray),
	currentPage_(0)
{
	backgroundColor_ = Color(0.05, 0.1, 0.05, 0.9);
	borderWidth_ = 2;
	padding_ = 2;
}

SubspaceHelp::~SubspaceHelp()
{
}

Uint SubspaceHelp::getDrawHeight() const
{
	return (getHeight() * font_.getFontHeight() + 2*padding_ + borderWidth_);
}

Uint SubspaceHelp::getDrawWidth() const
{
	return (getWidth() * font_.getFontWidth() + 2*padding_  + borderWidth_);
}

Uint SubspaceHelp::getWidth() const
{
	if(pages_.size() == 0)
		return 0;
	else
		return pages_[currentPage_].width;
}

Uint SubspaceHelp::getHeight() const
{
	if(pages_.size() == 0)
		return 0;
	else
		return (Uint)pages_[currentPage_].lines.size();
}

void SubspaceHelp::setFont(const TextureFont& font)
{
	font_ = font;
}

void SubspaceHelp::setPage(Uint page)
{
	assert(page < pages_.size());

	currentPage_ = min(pages_.size()-1, max(0, page));	//make sure page is in range
}

Uint SubspaceHelp::size() const
{	
	return (Uint)pages_.size();
}

bool SubspaceHelp::load(const string& filename)
{
	std::ifstream file(filename.c_str());
	if(!file.is_open())
		return false;

	string line;
	while(!file.eof())
	{
		std::getline(file, line, '\n');

		parseLine(line);
	}
	
	file.close();

	return true;
}

void SubspaceHelp::draw() const
{
	if(pages_.size() == 0)
		return;

	glDisable(GL_TEXTURE_2D);
	drawBackground();
		
	glColor4d(1.0, 1.0, 1.0, 1.0);
	glPushMatrix();
		glTranslated(padding_ + borderWidth_/2.0, padding_ + borderWidth_/2.0, 0);
		drawText();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	drawBorder();
}

void SubspaceHelp::drawBackground() const
{
	double drawWidth = getWidth() * font_.getFrameWidth();
	double drawHeight = getHeight() * font_.getFrameHeight();

	glColor4d(backgroundColor_.r_, backgroundColor_.g_, backgroundColor_.b_, backgroundColor_.a_);

	glBegin(GL_QUADS);			//background
		glVertex3d(drawWidth + 2*padding_ + borderWidth_/2.0, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, drawHeight + 2*padding_ + borderWidth_/2.0, 0);
		glVertex3d(drawWidth + 2*padding_ + borderWidth_/2.0, drawHeight + 2*padding_ + borderWidth_/2.0, 0);
	glEnd();
}

void SubspaceHelp::drawText() const
{
	Uint width;
	Uint i;

	if(pages_.size() == 0)
		return;

	const Page& page = pages_[currentPage_];

	glTranslated(0, font_.getFontHeight() * page.lines.size(), 0);
	for(i = 0; i < page.lines.size(); ++i)
	{
		width = 0;
		const ChunkList& line = page.lines[i];

		glTranslated(0, -(double)font_.getFontHeight(), 0);

		ChunkList::const_iterator j;
		for(j = line.begin(); j != line.end(); ++j)
		{
			const Chunk& c = (*j);
			font_.displayString(c.text.c_str(), c.color);
			width += c.text.size();

			glTranslated(c.text.size() * font_.getFontWidth(), 0, 0);
		}
		glTranslated(-(double)width * font_.getFontWidth(), 0, 0);
	}
}

void SubspaceHelp::drawBorder() const
{
	double drawWidth = getWidth() * font_.getFrameWidth();
	double drawHeight = getHeight() * font_.getFrameHeight();

	glColor4d(0.5, 0.5, 0.5, 1.0);
	glLineWidth(borderWidth_);
	glBegin(GL_LINE_LOOP);			//main border
		glVertex3d(0, 0, 0);
		glVertex3d(drawWidth + 2*padding_ + borderWidth_, 0, 0);
		glVertex3d(drawWidth + 2*padding_ + borderWidth_, drawHeight + 2*padding_ + borderWidth_, 0);
		glVertex3d(0, drawHeight + 2*padding_ + borderWidth_, 0);
		glVertex3d(0, 0, 0);
	glEnd();

	/*glColor4d(1.0, 1.0, 1.0, 1.0);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);			//main border
		glVertex3d(0, 0, 0);
		glVertex3d(drawWidth + 2*padding_ + borderWidth_, 0, 0);
		glVertex3d(drawWidth + 2*padding_ + borderWidth_, drawHeight + 2*padding_ + borderWidth_, 0);
		glVertex3d(0, drawHeight + 2*padding_ + borderWidth_, 0);
		glVertex3d(0, 0, 0);
	glEnd();*/

	double height = (getHeight()-1) * font_.getFontHeight();

	glLineWidth(1);
	glBegin(GL_LINES);				//header separator
		glVertex3d(0, height + padding_ + borderWidth_/2.0, 0);
		glVertex3d(drawWidth + 2*padding_ + borderWidth_/2.0, height + padding_ + borderWidth_/2.0, 0);
	glEnd();

	glColor4d(1.0, 1.0, 1.0, 1.0);
}

void SubspaceHelp::update(double time)
{
}

void SubspaceHelp::cacheChunk(const Chunk& c)
{
	if(c.text.size() == 0)
		return;

	if(!currentLine_)
		cacheNewline();

	currentLine_->push_back(c);
	lineWidth_ += c.text.size();

	if(lineWidth_ > pages_[currentPage_].width)
		pages_[currentPage_].width = lineWidth_;
}

void SubspaceHelp::cacheNewline()
{
	if(pages_.size() == 0)
		cacheNewpage();

	Page& page = pages_.back();

	ChunkList newline;

	page.lines.push_back(newline);
	currentLine_ = &page.lines.back();
	lineWidth_ = 0;
}

void SubspaceHelp::cacheNewpage()
{
	Page newPage;

	pages_.push_back(newPage);
	currentLine_ = 0;
	currentPage_ = pages_.size()-1;
}

Uint SubspaceHelp::changeState(const string& state)
{
	Uint stateSize = 0;

	if(state.size() == 0)
		return stateSize;

	//incorrect
	//string word = AsciiUtil::getWord(state, 0, AsciiUtil::Whitespace + "%", true);

	string word;
	word += state[0];

	if(word == "b")
	{
		currentColor_ = COLOR_Orange;
		stateSize = word.size();
	}
	else if(word == "B")
	{
		currentColor_ = COLOR_Gray;
		stateSize = word.size();
	}
	else if(word == "G")
	{
		currentColor_ = COLOR_Green;
		stateSize = word.size();
	}
	else if(word == "p")
	{
		currentColor_ = COLOR_Pink;
		stateSize = word.size();
	}
	else if(word == "Y")
	{
		currentColor_ = COLOR_Yellow;
		stateSize = word.size();
	}
	else if(word == "W")
	{
		currentColor_ = COLOR_Gray;
		stateSize = word.size();
	}

	return stateSize;
}

void SubspaceHelp::parseLine(const string& l)
{
	size_t offset = 0, lastOffset = 0;
	Uint stateSize = 0;
	string line = l;

	if(line.size() > 0 && line[0] == newPageChar)
	{
		cacheNewpage();
		++offset;
	}
	
	cacheNewline();

	lastOffset = offset;
	offset = line.find_first_of(newStateChar, offset);
	while(offset != line.npos && offset < line.size())	
	{
		cacheChunk(Chunk(line.substr(lastOffset, offset-lastOffset), currentColor_));	//cache chunk

		stateSize = changeState(line.substr(offset+1));
		if(stateSize > 0)
			offset += stateSize;
		else
		{
			string temp;
			temp += newStateChar;
			cacheChunk(Chunk(temp, currentColor_));	//write the '%' back in
		}
		++offset;
		
		lastOffset = offset;
		offset = line.find_first_of(newStateChar, offset);
	}

	offset = min(line.size(), offset);	//check for bad offsets
	if(offset-lastOffset > 0)	//leftover word
		cacheChunk(Chunk(line.substr(lastOffset, offset-lastOffset), currentColor_));
}