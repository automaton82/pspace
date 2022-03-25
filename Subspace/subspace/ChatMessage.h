// David Millman

#ifndef _CHATMESSAGE_H_
#define _CHATMESSAGE_H_

#include "DataTypes.h"

#include "ChatSettings.h"

struct ChatMessage
{
	ChatMessage();
	ChatMessage(string Sender, string Text, ChatType Type, Uint8 soundByte = 0);

	void setMessage(const string& sender, const string& text);	//parses message for type

	int getNameColor() const;
	int getTextColor() const;
	static int getMessageColor(const string& s);

	bool hasSender() const;

public:
	string sender;
	string text;

	ChatType type;
	Uint8 soundByte;
};

#endif