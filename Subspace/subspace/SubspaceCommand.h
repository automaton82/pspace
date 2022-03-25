#ifndef _SUBSPACECOMMAND_H_
#define _SUBSPACECOMMAND_H_	

#include <string>
using std::string;

//TODO: make a CHAT command simply a subset of all Subspace Commands
enum CommandType
{
	COMMAND_Unknown = 0,

	COMMAND_ArenaChange,
	COMMAND_ArenaLeave,
	COMMAND_Chat,

	COMMAND_Query_ConfigVar,
	COMMAND_Query_MessageLines,
	COMMAND_Query_Namelength,
	//COMMAND_Query_Nametag,
	//COMMAND_Query_StatBoxSize,

	COMMAND_Set_ConfigVar,
	COMMAND_Set_MessageLines,
	COMMAND_Set_Namelength,
	//COMMAND_Set_Nametag,
	//COMMAND_Set_StatBoxSize,

	COMMAND_ShipChange,
	COMMAND_TeamChange
};

struct ChatCommand
{
	ChatCommand()
		: type(COMMAND_Unknown)
	{}

	ChatCommand(CommandType comm, const string& param = "") 
		: type(comm), parameter(param)
	{}

	CommandType type;
	string parameter;
};

//////////////////////

ChatCommand getChatCommand(const string& message);

#endif