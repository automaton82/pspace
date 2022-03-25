#ifndef _SUBSPACEUSERACTIONRECEIVER_H_
#define _SUBSPACEUSERACTIONRECEIVER_H_

#include "InputEvent.h"

#include "InputCommands.h"

class SubspaceUserActionReceiver : 
	public CommandListenerBase<SubspaceUserActionReceiver, SubspaceUserAction>
{
public:
	
	// chat/info box commands
	virtual void doScrollUp();
	virtual void doScrollDown();
	virtual void doPageUp();
	virtual void doPageDown();

	// player weapon commands
	virtual void doFireBrick();
	virtual void doFireBomb();
	virtual void doFireBullet();
	virtual void doFireBurst();
	virtual void doFireDecoy();
	virtual void doFireMine();
	virtual void doFirePortal();
	virtual void doFireRepel();
	virtual void doFireRocket();
	virtual void doFireThor();

	// player movement commands
	virtual void doMoveForward();
	virtual void doMoveBackward();
	virtual void doTurnLeft();
	virtual void doTurnRight();
	virtual void doFastForward();		// fast are for afterburners / spectator
	virtual void doFastBackward();
	virtual void doFastLeft();
	virtual void doFastRight();

	// player toggleable states	
	virtual void doToggleAntiwarp();	// TODO: should these be toggleable or have on/off states?
	virtual void doToggleCloak();
	virtual void doToggleMultifire();
	virtual void doToggleStealth();
	virtual void doToggleXRadar();

	// misc
	virtual void doAttach();	// turret attachment
    virtual void doWarp();		

	// menu commands
	virtual void doQuit();
	//virtual void doShipChange();
	virtual void doToggleHelp();
	virtual void doToggleInfoBox();
	virtual void doToggleMenu();
	virtual void doToggleRadar();
};

#endif
