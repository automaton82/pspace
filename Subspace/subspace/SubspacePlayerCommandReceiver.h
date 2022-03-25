#ifndef _SUBSPACEPLAYERCOMMANDRECEIVER_H_
#define _SUBSPACEPLAYERCOMMANDRECEIVER_H_

#include "CommandListenerBase.h"

#include "InputEvent.h"
#include "InputCommands.h"

#include "SubspacePlayerCommands.h"

class SubspacePlayerCommandReceiver : 
	public CommandListenerBase<SubspacePlayerCommandReceiver, SubspacePlayerCommand>
{
public:
	
	virtual void doFireBrick() {}
	virtual void doFireBomb() {}
	virtual void doFireBullet() {}
	virtual void doFireBurst() {}
	virtual void doFireDecoy() {}
	virtual void doFireMine() {}
	virtual void doFirePortal() {}
	virtual void doFireRepel() {}
	virtual void doFireRocket() {}
	virtual void doFireThor() {}

	//movement
	virtual void doMoveForward() {}
	virtual void doMoveBackward() {}
	virtual void doTurnLeft() {}
	virtual void doTurnRight() {}
	virtual void doFastForward() {}		//afterburner
	virtual void doFastBackward() {}
	virtual void doFastLeft() {}				
	virtual void doFastRight() {}

	//toggles
	virtual void doToggleAntiwarp() {}
	virtual void doToggleCloak() {}
    virtual void doToggleMultifire() {}
	virtual void doToggleStealth() {}
	virtual void doToggleXRadar() {}

	//misc
	virtual void doAttach() {}	//turret attachment
	virtual void doWarp() {}
	
	//ship changes
	virtual void doChangeShip(int shipID) {}
	virtual void doChangeTeam(int teamID) {}

};

#endif
