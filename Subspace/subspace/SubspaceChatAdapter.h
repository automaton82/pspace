#ifndef _SUBSPACECHATADAPTER_H_
#define _SUBSPACECHATADAPTER_H_

#include "CommandGenerator.h"

#include "SubspaceDefines.h"
#include "SubspaceChatCommandReceiver.h"

class SubspaceChat;
class SubspacePlayer;

class SubspaceChatAdapter : 
	public SubspaceChatCommandReceiver,
	public CommandGenerator<SubspaceChatCommandReceiver>
{
public:

	SubspaceChatAdapter();

	// Mutators
	void setChat(SubspaceChat* chat);
	void setMyPlayer(const SubspacePlayer* myPlayer);
	void setPlayers(const SubspacePlayerMap* map);

	///////////////////////////////////

	virtual void doChat(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte);
	virtual void doServerChat(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte);

private:

	SubspaceChat* chat_;
	const SubspacePlayerMap* players_;
	const SubspacePlayer* myPlayer_;
};

#endif