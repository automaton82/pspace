#include "SubspaceMenuCommands.h"

#include "SubspaceMenuCommandReceiver.h"

SubspaceMenuCommand::~SubspaceMenuCommand()
{
}

void SubspaceMenuCommand::execute(SubspaceMenuCommandReceiver* receiver)
{
}

namespace SubspaceMenuCommands
{
	///////////////////////////////////////
	#define basicCommandImpl(name)								\
	void name::execute(SubspaceMenuCommandReceiver* receiver)	\
	{															\
		receiver->do##name();									\
	}																	
	///////////////////////////////////////

	basicCommandImpl(ActivateToggle); 
	basicCommandImpl(Quit);
	basicCommandImpl(Help);
	basicCommandImpl(StatBox);
	basicCommandImpl(NameTags);
	basicCommandImpl(Radar);
	basicCommandImpl(Messages);
	basicCommandImpl(HelpTicker);
	basicCommandImpl(EngineSounds);
	basicCommandImpl(DecreaseStatBoxSize);
	basicCommandImpl(IncreaseStatBoxSize);
	basicCommandImpl(ArenaList);
	basicCommandImpl(SetBanner);
	basicCommandImpl(IgnoreMacros);
	basicCommandImpl(ShipWarbird);
	basicCommandImpl(ShipJavelin);
	basicCommandImpl(ShipSpider);
	basicCommandImpl(ShipLeviathan);
	basicCommandImpl(ShipTerrier);
	basicCommandImpl(ShipWeasel);
	basicCommandImpl(ShipLancaster);
	basicCommandImpl(ShipShark);
	basicCommandImpl(Spectator);
}