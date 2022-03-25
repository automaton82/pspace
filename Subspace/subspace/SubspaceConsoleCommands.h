#ifndef _SUBSPACECONSOLECOMMANDS_H_
#define _SUBSPACECONSOLECOMMANDS_H_

class SubspaceConsoleCommandReceiver;

class SubspaceConsoleCommand
{
public:
	virtual ~SubspaceConsoleCommand();

	virtual void execute(SubspaceConsoleCommandReceiver* listener) = 0;
};

namespace SubspaceConsoleCommands
{
	/*class SaveMacro	: public SubspaceConsoleCommand
	{																	
	public: 
		void execute(SubspaceConsoleCommandReceiver* receiver);

		int macroID;
	};*/

	class SendMacro	: public SubspaceConsoleCommand
	{																	
	public: 
		SendMacro(int macroID);
		void execute(SubspaceConsoleCommandReceiver* receiver);

		int macroID;
	};

	class WriteChar	: public SubspaceConsoleCommand
	{																	
	public: 
		void execute(SubspaceConsoleCommandReceiver* receiver);

		char c;
	};

	// NOTE: these actions intentionally have no parameters, since any user input is based on one key
	///////////////////////////////////////
	#define basicConsoleCommand(name)												\
		class name															\
			: public SubspaceConsoleCommand									\
		{																	\
			public: void execute(SubspaceConsoleCommandReceiver* receiver);	\
		}				
	///////////////////////////////////////

	basicConsoleCommand(ClearBuffer);
	basicConsoleCommand(SendBuffer); 

	basicConsoleCommand(ScrollHistoryUp); 
	basicConsoleCommand(ScrollHistoryDown); 

	basicConsoleCommand(TabComplete); 
}

#endif