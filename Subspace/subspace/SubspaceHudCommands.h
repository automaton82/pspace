#ifndef _SUBSPACEHUDCOMMANDS_H_
#define _SUBSPACEHUDCOMMANDS_H_

class SubspaceHudCommandReceiver;

class SubspaceHudCommand
{
public:
	virtual ~SubspaceHudCommand() {}

	virtual void execute(SubspaceHudCommandReceiver* listener) = 0;
};

namespace SubspaceHudCommands
{
	// NOTE: these actions intentionally have no parameters, since any user input is based on one key
	///////////////////////////////////////
	#define basicHudCommand(name)											\
		class name															\
			: public SubspaceHudCommand										\
		{																	\
			public:	void execute(SubspaceHudCommandReceiver* receiver);		\
		}				
	///////////////////////////////////////

	//info box
	basicHudCommand(ScrollUp);
	basicHudCommand(ScrollDown);
	basicHudCommand(ScrollUpFast);
	basicHudCommand(ScrollDownFast);
	basicHudCommand(DecreaseStatBoxSize);
	basicHudCommand(IncreaseStatBoxSize);

	//menu commands
	basicHudCommand(ArenaList);		
	basicHudCommand(ToggleHelp);		
	basicHudCommand(ToggleHelpTicker);		
	basicHudCommand(ToggleStatBox);
	basicHudCommand(ToggleNametags);
	basicHudCommand(ToggleRadar);
	basicHudCommand(ToggleBigRadar);
}

#endif