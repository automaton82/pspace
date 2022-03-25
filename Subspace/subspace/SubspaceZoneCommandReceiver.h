#ifndef _SUBSPACEZONECOMMANDRECEIVER_H_
#define _SUBSPACEZONECOMMANDRECEIVER_H_

#include "InputEvent.h"

#include "InputCommands.h"

class SubspaceZoneCommandReceiver : 
	public CommandListenerBase<SubspaceZoneCommandReceiver, SubspaceZoneCommand>
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
};

#endif
