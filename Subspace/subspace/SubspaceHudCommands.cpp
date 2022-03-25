#include "SubspaceHudCommands.h"

#include "SubspaceHudCommandReceiver.h"

namespace SubspaceHudCommands
{
	// NOTE: these actions intentionally have no parameters, since any user input is based on one key
	///////////////////////////////////////
	#define basicHudCommandImpl(name)							\
	void name::execute(SubspaceHudCommandReceiver* receiver)	\
	{															\
		receiver->do##name();									\
	}																	
	///////////////////////////////////////

	//info box
	basicHudCommandImpl(ScrollUp);
	basicHudCommandImpl(ScrollDown);
	basicHudCommandImpl(ScrollUpFast);
	basicHudCommandImpl(ScrollDownFast);
	basicHudCommandImpl(DecreaseStatBoxSize);
	basicHudCommandImpl(IncreaseStatBoxSize);

	//menu commands
	basicHudCommandImpl(ArenaList);		
	basicHudCommandImpl(ToggleHelp);		
	basicHudCommandImpl(ToggleHelpTicker);		
	basicHudCommandImpl(ToggleStatBox);
	basicHudCommandImpl(ToggleNametags);
	basicHudCommandImpl(ToggleRadar);
	basicHudCommandImpl(ToggleBigRadar);
}

