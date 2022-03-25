#ifndef _SUBSPACEMENUCOMMANDRECEIVER_H_
#define _SUBSPACEMENUCOMMANDRECEIVER_H_

//#include "InputEvent.h"

//#include "InputCommands.h"
#include "CommandListenerBase.h"

#include "SubspaceMenuCommands.h"

class SubspaceMenuCommandReceiver;

class SubspaceMenuCommandReceiver : 
	public CommandListenerBase<SubspaceMenuCommandReceiver, SubspaceMenuCommand>
{
public:
	
	virtual void doActivateToggle(); 
	virtual void doQuit();
	virtual void doHelp();
	virtual void doStatBox();
	virtual void doNameTags();
	virtual void doRadar();
	virtual void doMessages();
	virtual void doHelpTicker();
	virtual void doEngineSounds();
	virtual void doDecreaseStatBoxSize();
	virtual void doIncreaseStatBoxSize();
	virtual void doArenaList();
	virtual void doSetBanner();
	virtual void doIgnoreMacros();
	virtual void doShipWarbird();
	virtual void doShipJavelin();
	virtual void doShipSpider();
	virtual void doShipLeviathan();
	virtual void doShipTerrier();
	virtual void doShipWeasel();
	virtual void doShipLancaster();
	virtual void doShipShark();
	virtual void doSpectator();
};

#endif
