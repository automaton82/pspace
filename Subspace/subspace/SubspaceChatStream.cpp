#include "SubspaceChatStream.h"

#include "SubspaceChat.h"

SubspaceChatStream::SubspaceChatStream() : 
	buffer_(0),
	currentColor_(0)
{
}

SubspaceChatStream::SubspaceChatStream(SubspaceChat* buffer) : 
	buffer_(buffer),
	currentColor_(0)
{
}

void SubspaceChatStream::setColor(Uchar color)
{
	currentColor_ = color;
}

void SubspaceChatStream::setChatBuffer(SubspaceChat* buffer)
{
	buffer_ = buffer;
}

void SubspaceChatStream::flush()
{
	/*Uint pos = currentBuffer_.find_first_of('\n');
	if(pos == currentBuffer_.npos || pos == currentBuffer_.size()-1)
	{
		buffer_->appendMessage(currentBuffer_, currentColor_);

		if(pos == currentBuffer_.size()-1)
			buffer_->cacheNewline(0)cacheNewline(0);
	}
	else
		buffer_->writeMessage("", 0, currentBuffer_, currentColor_);*/
	//buffer_->writeMessage("", 0, currentBuffer_, currentColor_);
	currentBuffer_.clear();
}

void SubspaceChatStream::put(char c)
{
	if(buffer_)
	{
		string temp;
		temp += c;
		currentBuffer_ += c;
		//buffer_->writeMessage("", 0, temp, currentColor_);
	}
}

void SubspaceChatStream::write(const char* s, int size)
{
	if(buffer_)
	{
		string str(s);
		str = str.substr(0, size);
		currentBuffer_ += str;
		//buffer_->writeMessage("", 0, str, currentColor_);
	}
}

void SubspaceChatStream::write(const string& s)
{
	if(buffer_)
	{
		currentBuffer_ += s;
		//buffer_->writeMessage("", 0, s, currentColor_);
	}
}