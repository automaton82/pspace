//David Millman

#ifndef _TIMEDTEXTBOX_H_
#define _TIMEDTEXTBOX_H_

#include "TextBox.h"

class TimedTextBox : 
	public TextBox
{
public:
	TimedTextBox();
	TimedTextBox(Uint width, Uint height, double messageTime = 0);
	virtual ~TimedTextBox();

	//settings
	void setMessageTime(double milliseconds);
	void setScroll(bool scroll);
	
	//add text
	virtual void addWord(const string& s, int color);	//0 length *lines* are assumed to be non-messages

	virtual void newLine();
 
	//update - for timed messages
	void update(double timeStep);

protected:
	
	struct TimeBlock			//TODO: use color indexes for color (i.e. per letter)?
	{
		TimeBlock() : timeLeft(0), blockList(0)
		{}

		TimeBlock(double time, TextBlockList* list) : timeLeft(time), blockList(list)
		{}

		double timeLeft;
		TextBlockList* blockList;
	};

	typedef list<TimeBlock> TimeList;

protected:

	bool findBlockList(TextBlockList* list);

	const double INFINITE_TIME;
	TimeList times_;

	bool doScroll_;
	double messageTime_;
};

#endif