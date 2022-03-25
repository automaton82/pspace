#ifndef _SUBSPACEMENUCOMMANDS_H_
#define _SUBSPACEMENUCOMMANDS_H_

class SubspaceMenuCommandReceiver;

class SubspaceMenuCommand
{
public:
	virtual ~SubspaceMenuCommand();

	virtual void execute(SubspaceMenuCommandReceiver* listener) = 0;
};

namespace SubspaceMenuCommands
{
	// NOTE: these actions intentionally have no parameters, since any user input is based on one key
	///////////////////////////////////////
	#define basicMenuCommand(name)												\
		class name															\
			: public SubspaceMenuCommand									\
		{																	\
			public: void execute(SubspaceMenuCommandReceiver* receiver);	\
		}				
	///////////////////////////////////////

    basicMenuCommand(ActivateToggle); 
	basicMenuCommand(Quit);
	basicMenuCommand(Help);
	basicMenuCommand(StatBox);
	basicMenuCommand(NameTags);
	basicMenuCommand(Radar);
	basicMenuCommand(Messages);
	basicMenuCommand(HelpTicker);
	basicMenuCommand(EngineSounds);
	basicMenuCommand(DecreaseStatBoxSize);
	basicMenuCommand(IncreaseStatBoxSize);
	basicMenuCommand(ArenaList);
	basicMenuCommand(SetBanner);
	basicMenuCommand(IgnoreMacros);
	basicMenuCommand(ShipWarbird);
	basicMenuCommand(ShipJavelin);
	basicMenuCommand(ShipSpider);
	basicMenuCommand(ShipLeviathan);
	basicMenuCommand(ShipTerrier);
	basicMenuCommand(ShipWeasel);
	basicMenuCommand(ShipLancaster);
	basicMenuCommand(ShipShark);
	basicMenuCommand(Spectator);
}

#endif