#ifndef _SUBSPACECHATCOMMANDRECEIVER_H_
#define _SUBSPACECHATCOMMANDRECEIVER_H_

#include <string>
using std::string;

#include "DataTypes.h"
#include "CommandListenerBase.h"

#include "SubspaceChatCommands.h"

class SubspaceChatCommandReceiver :
	public CommandListenerBase<SubspaceChatCommandReceiver, SubspaceChatCommand>
{
public:

	//0x07	- on client and server
	virtual void doChat(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte) {}
	virtual void doServerChat(Uint16 playerID, const string& message, Uint8 chatType, Uint8 soundByte) {}
};

#endif