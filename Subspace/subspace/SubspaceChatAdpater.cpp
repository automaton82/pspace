#include "SubspaceChatAdapter.h"

#include "SubspaceChat.h"
#include "SubspacePlayer.h"

SubspaceChatAdapter::SubspaceChatAdapter() : 
	chat_(0),
	myPlayer_(0),
	players_(0)
{
}
	
void SubspaceChatAdapter::setChat(SubspaceChat* chat)
{
	chat_ = chat;
}

void SubspaceChatAdapter::setPlayers(const SubspacePlayerMap* map)
{
	players_ = map;
}

void SubspaceChatAdapter::setMyPlayer(const SubspacePlayer* player)
{
	myPlayer_ = player;
}

void SubspaceChatAdapter::doChat(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte)
{
	if(!chat_)
		return;

	string name = "";
	SubspacePlayerMap::const_iterator i;
	if(players_)
	{
		i = players_->find(playerID);
		if(i != players_->end())
			name = ((*i).second)->getName();
	}
		
	chat_->addMessage(ChatMessage(name, message, (ChatType)chatType, soundByte));
}

void SubspaceChatAdapter::doServerChat(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte)
{
	string name = "<unknown>";
	
	if(myPlayer_)
		name = myPlayer_->getName();
	if(chatType == CHAT_Private)
	{
		SubspacePlayerMap::const_iterator i;
		if(players_)
		{
			i = players_->find(playerID);
			if(i != players_->end())
				name = "->" + ((*i).second)->getName();
		}
	}

	chat_->addMessage(ChatMessage(name, message, (ChatType)chatType, soundByte));
		
	SubspaceChatCommands::ServerChat cmd;		
	cmd.playerID = playerID;
	cmd.message = message;
	cmd.type = (ChatType)chatType;
	cmd.soundByte = soundByte;

	this->CommandGenerator<SubspaceChatCommandReceiver>::invokeCommand(&cmd);
}
