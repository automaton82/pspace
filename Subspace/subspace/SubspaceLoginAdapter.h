#ifndef _SUBSPACELOGINADAPTER_H_
#define _SUBSPACELOGINADAPTER_H_

#include "CommandGenerator.h"
#include "SubspaceClientCommandListener.h"
#include "SubspaceServerCommandListener.h"

class SubspaceLoginAdapter : 
	public SubspaceClientCommandListener,
	public CommandGenerator<SubspaceServerCommandListener>
{
public:
	
	void login(const string& username, const string& password);

	//0x0A											
	virtual void doPasswordResponse(BYTE response, const string& message, Uint32 version, BYTE regForm, Uint32 newsChecksum);

private:


};

#endif
