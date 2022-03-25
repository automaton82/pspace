#ifndef _SUBSPACEHUDCOMMANDRECEIVER_H_
#define _SUBSPACEHUDCOMMANDRECEIVER_H_

#include "CommandListenerBase.h"
#include "InputEvent.h"
#include "InputCommands.h"

#include "SubspaceHudCommands.h"

class SubspaceHudCommandReceiver : 
	public CommandListenerBase<SubspaceHudCommandReceiver, SubspaceHudCommand>
{
public:
	
	//info box
	virtual void doScrollUp() {}
	virtual void doScrollDown() {}
	virtual void doScrollUpFast() {}
	virtual void doScrollDownFast() {}
	virtual void doDecreaseStatBoxSize() {}
	virtual void doIncreaseStatBoxSize() {}

	//menu commands
	virtual void doArenaList() {}		
	virtual void doToggleHelp() {}		
	virtual void doToggleHelpTicker() {}		
	virtual void doToggleStatBox() {}
	virtual void doToggleNametags() {}
	virtual void doToggleRadar() {}
	virtual void doToggleBigRadar() {}
};

#endif