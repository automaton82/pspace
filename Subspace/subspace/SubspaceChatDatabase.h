// David Millman

#ifndef _SUBSPACECHATDATABASE_H_
#define _SUBSPACECHATDATABASE_H_

#include "Database.h"
#include "ChatData.h"

class SubspaceChatDatabase :
	public Database<ChatData>
{
public:

	string getSender(int id);
	string getText(int id);
	ChatType getMessageType(int id);

private:

};

#endif