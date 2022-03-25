#ifndef _SUBSPACECHATCOMMANDS_H_
#define _SUBSPACECHATCOMMANDS_H_

#include <string>
using std::string;

#include "DataTypes.h"
#include "ChatSettings.h"

class SubspaceChatCommandReceiver;

class SubspaceChatCommand
{
public:
	virtual ~SubspaceChatCommand();

	virtual void execute(SubspaceChatCommandReceiver* commandListener) = 0;
};

namespace SubspaceChatCommands
{

//0x07
	struct Chat : public SubspaceChatCommand
	{
		void execute(SubspaceChatCommandReceiver* listener);		

		Uint16 playerID;	// TODO: verify this, playerID, teamID
		string message;
		ChatType type;		// TODO: verify this, squad chat?
		Uint8 soundByte;
	};

	struct ServerChat : public SubspaceChatCommand
	{
		void execute(SubspaceChatCommandReceiver* listener);		

		Uint16 playerID;	// TODO: verify this, playerID, teamID
		string message;
		ChatType type;		// TODO: verify this, squad chat?
		Uint8 soundByte;
	};
}


#endif