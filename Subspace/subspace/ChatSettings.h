#ifndef _CHATSETTINGS_H_
#define _CHATSETTINGS_H_

#include <string>
using std::string;


// Chat parameters
// c2s (Chat message) 06 __ 00 00 00 MM EE SS SS AA GG EE 00
enum ChatType
{
    CHAT_Arena,			// 00 Same for *arena, *zone, *szone, **
    CHAT_PublicMacro,	// 01 Macro calcs are done client-side
    CHAT_Public,		// 02 Public chat
    CHAT_Team,			// 03 (//) or (')
    CHAT_TeamPrivate,	// 04 Player to all members of another team (")
    CHAT_Private,		// 05 Only shows messages addressed to the bot
    CHAT_PlayerWarning,	// 06 Red message, with a name tag
    CHAT_RemotePrivate,	// 07 Do not trust the sender with any private information.
    CHAT_ServerError,	// 08 Red server errors, without a name tag
	CHAT_Channel,		// 09 Arrives in the same format SubSpace displays

	CHAT_Query,
	CHAT_Unknown
};

#endif