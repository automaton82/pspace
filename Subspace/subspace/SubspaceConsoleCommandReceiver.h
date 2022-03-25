#ifndef _SUBSPACECONSOLECOMMANDRECEIVER_H_
#define _SUBSPACECONSOLECOMMANDRECEIVER_H_

#include "CommandListenerBase.h"

#include "InputEvent.h"
#include "InputCommands.h"

#include "SubspaceConsoleCommands.h"

class SubspaceConsoleCommandReceiver : 
	public CommandListenerBase<SubspaceConsoleCommandReceiver, SubspaceConsoleCommand>
{
public:
	
	virtual void doWriteChar(char c);

	//virtual void doSaveMacro(int macroID);
	virtual void doSendMacro(int macroID);

	virtual void doClearBuffer();
	virtual void doSendBuffer(); 

	virtual void doScrollHistoryUp(); 
	virtual void doScrollHistoryDown(); 

	virtual void doTabComplete(); 
};

#endif