#ifndef _SUBSPACEUSERADAPTER_H_
#define _SUBSPACEUSERADAPTER_H_

#include "SubspaceUserActionReceiver.h"

class SubspaceChat;
class SubspaceZone;

// this class connects user actions/commands to the zone
class SubspaceUserAdapter : 
	public SubspaceUserActionReceiver
{
public:

	void setChat(SubspaceChat* chat);
	void setHud(SubspaceHud* hud);
	void setZone(SubspaceZone* zone);


	///////////////////////////////////
	
	// chat/info box commands
	void doScrollUp();
	void doScrollDown();
	void doPageUp();
	void doPageDown();
	void doSendChat();

	// player weapon commands
	void doFireBrick();
	void doFireBomb();
	void doFireBullet();
	void doFireBurst();
	void doFireDecoy();
	void doFireMine();
	void doFirePortal();
	void doFireRepel();
	void doFireRocket();
	void doFireThor();

	// player movement commands
	void doMoveForward();
	void doMoveBackward();
	void doTurnLeft();
	void doTurnRight();
	void doFastForward();		// fast are for afterburners / spectator
	void doFastBackward();
	void doFastLeft();
	void doFastRight();

	// player toggleable states	
	void doToggleAntiwarp();	// TODO: should these be toggleable or have on/off states?
	void doToggleCloak();
	void doToggleMultifire();
	void doToggleStealth();
	void doToggleXRadar();

	// misc
	void doAttach();	// turret attachment
	void doWarp();		

	// menu commands
	void doQuit();
	// void doShipChange();
	void doToggleHelp();
	void doToggleInfoBox();
	void doToggleMenu();
	void doToggleRadar();

private:

	SubspaceChat* chat_;
	SubspaceHud* hud_;
	SubspaceZone* zone_;
};

#endif