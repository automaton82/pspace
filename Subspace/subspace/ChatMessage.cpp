#include "ChatMessage.h"

#include "AsciiUtil.h"
#include "SubspaceSettings.h"

ChatMessage::ChatMessage()
{
}

ChatMessage::ChatMessage(string Sender, string Text, ChatType Type, Uint8 Sound) :
	sender(Sender), text(Text), type(Type), soundByte(Sound)
{
}

void ChatMessage::setMessage(const string& sender, const string& t)
{
	Uint n;
	int color = 0;

	this->sender = sender;

	n = t.find_first_not_of(AsciiUtil::Whitespace, 0);
	//for(n=0; t[n] == ' ' && n < t.size(); ++n);		//ignore spaces in front
    
	if(n < t.size())
	{
		switch(t[n])
		{
		case '\'':
			type = CHAT_Team;
			text = t.substr(n+1);
			break;
		case '/':
			type = CHAT_Private;
			text = t.substr(n+1);
	
			if(n+1 < t.size() && t[n+1] == '/')
			{
				type = CHAT_Team;
				text = t.substr(n+2);
			}

			break;
		case '"':
			type = CHAT_TeamPrivate;
			text = t.substr(n+1);
			break;
		case ':':
			type = CHAT_Private;
		case ';':
			type = CHAT_Channel;
		default:
			type = CHAT_Public;
			text = t;
		}
	}
}

bool ChatMessage::hasSender() const
{
	bool retval = true;

	switch(type)
	{
	case CHAT_Arena:		
		retval = false; break;
	case CHAT_PlayerWarning:	//not sure about these
		retval = false; break;
	case CHAT_ServerError:
		retval = false; break;
	case CHAT_Channel:
		retval = false; break;
	}

	return retval;
}

int ChatMessage::getNameColor() const
{
	int color = 0;		//0-gray, 1-green, 2-blue, 3-red, 4-yellow, 5-purple, 6-orange, 7-pink
	switch(type)
	{
	case CHAT_Arena:		// 00 Same for *arena, *zone, *szone, **
		color = COLOR_Green;	break;
	case CHAT_Public:		// 02 Public chat
	case CHAT_PublicMacro:
		color = COLOR_Blue;		break;
	case CHAT_Team:
		color = COLOR_Yellow;	break;
	case CHAT_TeamPrivate:	// 04 Player to all members of another team (")
		color = COLOR_Green;	break;
	case CHAT_Private:		// 05 Only shows messages addressed to the bot
		color = COLOR_Green;	break;
	case CHAT_PlayerWarning:
		color = COLOR_Red;		break;
	case CHAT_RemotePrivate:
		color = COLOR_Orange;	break;
	case CHAT_ServerError:
		color = COLOR_Red;		break;
	case CHAT_Channel:
		color = COLOR_Orange;	break;
	case CHAT_Query:
		color = COLOR_Pink;		break;
	default:
		color = COLOR_Gray;
	}
	return color;
}

int ChatMessage::getTextColor() const
{
	int color = 0;		//0-gray, 1-green, 2-blue, 3-red, 4-yellow, 5-purple, 6-orange, 7-pink
	switch(type)
	{
	case CHAT_Arena:		// 00 Same for *arena, *zone, *szone, **
		color = COLOR_Green;	break;
	case CHAT_Public:		// 02 Public chat
	case CHAT_PublicMacro:
		color = COLOR_Blue;		break;
	case CHAT_Team:
		color = COLOR_Yellow;	break;
	case CHAT_TeamPrivate:	// 04 Player to all members of another team (")
		color = COLOR_Blue;		break;
	case CHAT_Private:		// 05 Only shows messages addressed to the bot
		color = COLOR_Green;	break;
	case CHAT_PlayerWarning:
		color = COLOR_Red;		break;
	case CHAT_RemotePrivate:
		color = COLOR_Orange;	break;
	case CHAT_ServerError:
		color = COLOR_Red;		break;
	case CHAT_Channel:
		color = COLOR_Orange;	break;
	case CHAT_Query:
		color = COLOR_Pink;		break;
	default:
		color = 0;
	}
	return color;
}

int ChatMessage::getMessageColor(const string& s)
{
	Uint n;
	int color = 0;

	for(n=0; s[n] == ' ' && n < s.size(); ++n);		//ignore spaces in front
    
	if(n < s.size())
	{
		switch(s[n])
		{
		case '\'':
			color = 4;
			break;
		case '/':
			color = 1;

			if(n+1 < s.size() && s[n+1] == '/')
				color = 4;

			break;
		}
	}

	return color;
}