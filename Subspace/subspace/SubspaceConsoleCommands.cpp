#include "SubspaceConsoleCommands.h"

#include "SubspaceConsoleCommandReceiver.h"

SubspaceConsoleCommand::~SubspaceConsoleCommand()
{
}

namespace SubspaceConsoleCommands
{
	/*void SaveMacro::execute(SubspaceConsoleCommandReceiver* receiver)	
	{																
		receiver->doSaveMacro(macroID);
	}*/		

	SendMacro::SendMacro(int m) 
		: macroID(m)
	{
	}

	void SendMacro::execute(SubspaceConsoleCommandReceiver* receiver)	
	{																
		receiver->doSendMacro(macroID);
	}		

	void WriteChar::execute(SubspaceConsoleCommandReceiver* receiver)	
	{																
		receiver->doWriteChar(c);
	}	

	///////////////////////////////////////
	#define basicCommandImpl(name)									\
	void name::execute(SubspaceConsoleCommandReceiver* receiver)	\
	{																\
		receiver->do##name();										\
	}																		
	///////////////////////////////////////

	basicCommandImpl(ClearBuffer);
	basicCommandImpl(SendBuffer); 

	basicCommandImpl(ScrollHistoryUp); 
	basicCommandImpl(ScrollHistoryDown); 

	basicCommandImpl(TabComplete); 

}