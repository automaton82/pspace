#include "Console.h"

#include "AsciiUtil.h"

Console::Console() : 
	maxBufferSize_(defaultBufferSize),
	maxHistorySize_(defaultHistorySize)
{
	newCommand();
	currentCommand_ = lastCommand();
}

Console::~Console()
{
}

const string& Console::getBuffer() const
{
	return history_.back();
}

size_t Console::getMaxBufferSize() const
{
	return maxBufferSize_;
}

size_t Console::getMaxHistorySize() const
{
	return maxHistorySize_;
}

void Console::setMaxBufferSize(size_t size)
{
	maxBufferSize_ = size;
}

void Console::setMaxHistorySize(size_t size)
{
	maxHistorySize_ = size;
}

void Console::clear()
{
	buffer().clear();
}

void Console::nextCommand()
{
	if(++currentCommand_ == history_.end())
		--currentCommand_;

	cursorPosition_ = buffer().size();
}

void Console::previousCommand()
{
	if(currentCommand_ != history_.begin())
		--currentCommand_;

	cursorPosition_ = buffer().size();
}

void Console::write(char c)
{
	onPreInput();

	switch(c)
	{
	case '\b':
		inputBackspace();
		break;
	case '\n':
		inputEnter();
		break;
	case '\t':
		inputTab();
		break;
	default:
		if(AsciiUtil::isAlpha(c))
		{
			if(currentCommand_ != lastCommand())		// on previous command
			{											// copy command
				newCommand();
				currentCommand_ = lastCommand();
			}
			inputChar(c);
		}
	}

	onPostInput();
}

void Console::write(const string& s)
{
	string::const_iterator i;
	for(i = s.begin(); i != s.end(); ++i)
		write(*i);
}

string& Console::buffer()
{
	return history_.back();
}

void Console::inputBackspace()
{
	if(cursorPosition_ > 0)
	{
		buffer().erase(--cursorPosition_);
	}	
}

void Console::inputChar(char c)
{
	if(buffer().size() < maxBufferSize_)
		buffer().insert(cursorPosition_++, 1,	c);
}

void Console::inputEnd()
{
	cursorPosition_ = buffer().size();
}

void Console::inputEnter()
{
	// TODO: send input message
	newCommand();
}

void Console::inputHome()
{
	cursorPosition_ = 0;
}

void Console::inputTab()
{
	tabCompletion();
}

Console::StringList::iterator Console::lastCommand()
{
	return --(history_.end());
}
	
void Console::newCommand()
{
	if(buffer().size() > 0)
	{
		history_.push_back(string());
		cursorPosition_ = 0;
	}

	if(history_.size() > maxHistorySize_)
	{
		history_.pop_front();
	}
}

void Console::onPreInput()
{
}

void Console::onPostInput()
{
}

void Console::tabCompletion()
{
	// implemented by subclass
}
