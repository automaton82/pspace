//David Millman

#include "TimedTextBox.h"

TimedTextBox::TimedTextBox() : TextBox(), INFINITE_TIME(99999.0), doScroll_(false)
{
}

TimedTextBox::TimedTextBox(Uint width, Uint height, double messageTime) : 
	TextBox(width, height), 
	INFINITE_TIME(99999.0), 
	messageTime_(messageTime),
	doScroll_(false)
{
}

TimedTextBox::~TimedTextBox()
{
}

void TimedTextBox::setMessageTime(double milliseconds)
{
	messageTime_ = milliseconds;
}

void TimedTextBox::setScroll(bool scroll)
{
	doScroll_ = scroll;
}

void TimedTextBox::addWord(const string& s, int color)
{
	TextBlock newText(s, color);
	
	if(s.length() == 0)	//no word
		return;

	if(text_.size() == 0)
		newLine();

	int headerWidth = (int)header_.text.length();
	if(s.length() > width_)		//check for a multiple line word
	{
		for(unsigned int i=0; i < s.length(); i += width_-headerWidth)	//recursively add pieces
			addWord(s.substr(i, width_-headerWidth), color);
	}
	else
	{
		if(currentWidth_ + s.length() > width_-headerWidth)
			newLine();

		currentLine_->push_back(newText);

		//add time for update, per line only
		if(!findBlockList(currentLine_))
		{
			TimeBlock newTime(messageTime_, currentLine_);
	
			times_.push_back(newTime);
		}
	}

	currentWidth_ += (Uint)s.length();
}

void TimedTextBox::update(double timeStep)
{
	//timeStep *= 10000;	//TODO: make this a real timestep, not subspace dependent - done

	TimeList::iterator i;
	for(i = times_.begin(); i != times_.end(); ++i)
	{
		TimeBlock& time = (*i);

		time.timeLeft -= timeStep;
		
		if(time.timeLeft <= 0 && time.timeLeft != INFINITE_TIME)
		{						
			time.blockList->clear();	//clear the line
		}
	}
}

bool TimedTextBox::findBlockList(TextBlockList* list)
{
	TimeList::iterator i;
	for(i = times_.begin(); i != times_.end(); ++i)
	{
		TimeBlock& time = (*i);

		if(list == time.blockList)
			return true;
	}

	return false;
}

void TimedTextBox::newLine()
{
	if(doScroll_)
	{
		TextBox::newLine();
	}
	else
	{		//find first unused or lowest timeLeft line
		bool unused = false;
		TimeBlock* nextLine = 0;
	
		TimeList::iterator i;
		for(i = times_.begin(); i != times_.end(); ++i)
		{
			TimeBlock& time = (*i);
			if(time.blockList->size() == 0)		//unused line
			{
				unused = true;
				nextLine = &time;
				break;
			}
			else if(!unused)				//check for oldest line
			{		
				if(!nextLine)				//no line found yet
					nextLine = &time;
				else if(time.timeLeft < nextLine->timeLeft)	//older line
					nextLine = &time;
			}
		}

		if(!unused && text_.size() < height_)	//not all lines created yet
		{
			TextBox::newLine();
		}
		else
		{
			assert(nextLine);
			currentLine_ = nextLine->blockList;
			nextLine->timeLeft = messageTime_;

			currentLine_->clear();					//clear line
			currentWidth_ = 0;
		}
	}
}