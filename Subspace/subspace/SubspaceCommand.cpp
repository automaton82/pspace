#include "SubspaceCommand.h"

#include "AsciiUtil.h"
using namespace AsciiUtil;

ChatCommand getChatCommand(const string& msg)
{
	ChatCommand command;
		
	if(msg.size() == 0)
		return command;

	switch(msg[0])
	{
	case '=':
		command.type = COMMAND_TeamChange;
		command.parameter = getWord(msg, 1);
		break;
	case '?':
		string word = getWord(msg, 1, AsciiUtil::Whitespace + '=', true);
		word = tolower(word);
		
		if(word == "go")
		{
			command.type = COMMAND_ArenaChange;
			command.parameter = getWord(msg, 1+word.size(), AsciiUtil::Whitespace, true);
		}
		else if(word == "leave")
		{
			command.type = COMMAND_ArenaLeave;			
		}
		else if(word == "lines")
		{
			if(msg.size() > 1+word.size()+1 && msg[1+word.size()] == '=')
			{
				command.type = COMMAND_Set_MessageLines;
				command.parameter = getWord(msg, 1+word.size()+1, AsciiUtil::Whitespace, true);
			}
			else
			{
				command.type = COMMAND_Query_MessageLines;
			}
		}
		else if(word == "namelen")
		{
			if(msg.size() > 1+word.size() && msg[1+word.size()] == '=')
			{
				command.type = COMMAND_Set_Namelength;
				command.parameter = getWord(msg, 1+word.size()+1, AsciiUtil::Whitespace, true);
			}
			else
			{
				command.type = COMMAND_Query_Namelength;
			}
		}
		/*else if(word == "nametag")
		{
			if(msg.size() > 1+word.size() && msg[1+word.size()] == '=')
			{
				command.type = COMMAND_Set_Nametag;
				command.parameter = getWord(msg, 1+word.size()+1, AsciiUtil::Whitespace, true);
			}
			else
			{
				command.type = COMMAND_Query_Nametag;
			}
		}*/
		else if(word == "ship")
		{
			command.type = COMMAND_ShipChange;
			command.parameter = getWord(msg, 1 + word.size(), AsciiUtil::Whitespace, true);
		}
		else if(word == "team")
		{
			command.type = COMMAND_TeamChange;
			command.parameter = getWord(msg, 1 + word.size(), AsciiUtil::Whitespace, true);
		}
		else	//all config vars - case sensitive
		{
			word = getWord(msg, 1, AsciiUtil::Whitespace + '=', true);	//not lowercase

			if(msg.size() > 1+word.size() && msg[1+word.size()] == '=')	//setting config var
			{
				command.type = COMMAND_Set_ConfigVar;
				command.parameter = word + " " + getWord(msg, 1+word.size()+1, AsciiUtil::Whitespace, false);	//parameter = "var value"
			}
			else	//querying config var
			{
				command.type = COMMAND_Query_ConfigVar;
				command.parameter = word;
			}
		}
		break;
	}
	
	return command;
}