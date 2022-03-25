#include "SubspaceChatDatabase.h"

string SubspaceChatDatabase::getSender(int id)
{
	const ChatData* c = getItem(id);

	if(c)
		return c->sender;
	else
		return string();
}

string SubspaceChatDatabase::getText(int id)
{
	const ChatData* c = getItem(id);

	if(c)
		return c->text;
	else
		return string();
}

ChatType SubspaceChatDatabase::getMessageType(int id)
{
	const ChatData* c = getItem(id);

	if(c)
		return c->type;
	else
		return CHAT_Unknown;
}