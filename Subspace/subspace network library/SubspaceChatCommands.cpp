#include "SubspaceChatCommands.h"

#include "SubspaceChatCommandReceiver.h"

SubspaceChatCommand::~SubspaceChatCommand()
{
}

void SubspaceChatCommand::execute(SubspaceChatCommandReceiver* commandListener)
{
}

namespace SubspaceChatCommands
{

	//0x07
	void Chat::execute(SubspaceChatCommandReceiver* listener)
	{
		listener->doChat(playerID, message, type, soundByte);
	}

	void ServerChat::execute(SubspaceChatCommandReceiver* listener)
	{
		listener->doServerChat(playerID, message, type, soundByte);
	}

}