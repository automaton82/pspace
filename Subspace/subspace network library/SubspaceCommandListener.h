#ifndef _SUBSPACECOMMANDLISTENER_H_
#define _SUBSPACECOMMANDLISTENER_H_

class SubspaceCommand;

class SubspaceCommandListener
{
public:

	SubspaceCommandListener();
	virtual ~SubspaceCommandListener();

	virtual void handleCommand(SubspaceCommand& command)
	{
		command.execute(this);
	}
};

#endif