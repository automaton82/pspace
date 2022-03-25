#include "SubspaceHelp.h"

#include <fstream>
#include <iostream>
#include "SubspaceSettings.h"

SubspaceHelp::SubspaceHelp() : 
	currentColor_(COLOR_Gray),
	currentPage_(0)
{
	backgroundColor_ = Color(0.05, 0.1, 0.05, 0.9);
	borderWidth_ = 2;
	//padding_ = 2;

	cacheNewpage();
	//cacheNewpage();
	//currentPage_ = pages_.size()-1;
}

SubspaceHelp::~SubspaceHelp()
{
}

double SubspaceHelp::getDisplayHeight() const
{
	if(pages_.size() == 0)
		return 0;
	else
		return pages_[currentPage_].getDisplayHeight();
}

double SubspaceHelp::getDisplayWidth() const
{
	if(pages_.size() == 0)
		return 0;
	else
		return pages_[currentPage_].getDisplayHeight();
}

Uint SubspaceHelp::size() const
{	
	return (Uint)pages_.size();
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

	//pages_[currentPage_].print();

	currentPage_ = 0;

	return true;
}

void SubspaceHelp::nextPage()
{
	++currentPage_;
	if(currentPage_ >= pages_.size())
		currentPage_ = 0;
}

void SubspaceHelp::draw() const
{
	if(pages_.size() == 0)
		return;

	if(currentPage_ == 0)
		return;

	glColor4d(1.0, 1.0, 1.0, 1.0);
	pages_[currentPage_].draw();
}

void SubspaceHelp::update(double time)
{
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
		currentColor_ = COLOR_Blue;
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

void SubspaceHelp::cacheNewpage()
{
	SubspaceTextBox newPage;
	newPage.setFont(font_);
	newPage.setBackgroundColor(Color(10.0/255.0, 25.0/255.0, 10.0/255.0, 0.9));
	newPage.setPadding(1.0, 0.0, 1.0, 1.0);

	pages_.push_back(newPage);
}

void SubspaceHelp::parseLine(const string& l)
{
	size_t offset = 0, lastOffset = 0;
	Uint stateSize = 0;
	string line = l;
	string parsed;
	bool first = false;

	if(line.size() > 0 && line[0] == newPageChar)
	{
		cacheNewpage();
		++offset;
		currentPage_ = pages_.size()-1;

		/*if(currentPage_ > 0)
			pages_[currentPage_-1].print();*/

		first = true;
		currentColor_ = COLOR_Green;
	}

	if(pages_.size() == 0)
	{
		cacheNewpage();
		currentPage_ = pages_.size()-1;
	}
	
	if(pages_[currentPage_].size() > 0)
		pages_[currentPage_].write("\n");

	lastOffset = offset;
	offset = line.find_first_of(newStateChar, offset);
	while(offset != line.npos && offset < line.size())	
	{
		if(first)
			pages_[currentPage_].writeHeader(line.substr(lastOffset, offset-lastOffset), currentColor_);
		else
			pages_[currentPage_].write(line.substr(lastOffset, offset-lastOffset), currentColor_);	//cache chunk
		
		stateSize = changeState(line.substr(offset+1));
		if(stateSize > 0)
			offset += stateSize;
		else
		{
			string temp;
			temp += newStateChar;

			if(first)
				pages_[currentPage_].writeHeader(temp, currentColor_);	//cache chunk	
			else
				pages_[currentPage_].write(temp, currentColor_);	//cache chunk			
		}
		++offset;
		
		lastOffset = offset;
		offset = line.find_first_of(newStateChar, offset);
	}

	offset = min(line.size(), offset);	//check for bad offsets
	if(offset-lastOffset > 0)	//leftover word
	{
		if(first)
			pages_[currentPage_].writeHeader(line.substr(lastOffset, offset-lastOffset), currentColor_);
		else
			pages_[currentPage_].write(line.substr(lastOffset, offset-lastOffset), currentColor_);	//cache chunk		
	}

	pages_[currentPage_].flush();
}