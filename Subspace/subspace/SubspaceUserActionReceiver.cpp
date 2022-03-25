#include "SubspaceUserActionReceiver.h"

#define emptyActionFunction(name) void SubspaceUserActionReceiver::do##name() {}

emptyActionFunction(ScrollUp);
emptyActionFunction(ScrollDown);
emptyActionFunction(PageUp);
emptyActionFunction(PageDown);
emptyActionFunction(SendChat);

// player weapon commands
emptyActionFunction(FireBrick);
emptyActionFunction(FireBomb);
emptyActionFunction(FireBullet);
emptyActionFunction(FireBurst);
emptyActionFunction(FireDecoy);
emptyActionFunction(FireMine);
emptyActionFunction(FirePortal);
emptyActionFunction(FireRepel);
emptyActionFunction(FireRocket);
emptyActionFunction(FireThor);

// player movement commands
emptyActionFunction(MoveForward);
emptyActionFunction(MoveBackward);
emptyActionFunction(TurnLeft);
emptyActionFunction(TurnRight);
emptyActionFunction(FastForward);		// fast are for afterburners / spectator
emptyActionFunction(FastBackward);
emptyActionFunction(FastLeft);
emptyActionFunction(FastRight);

// player toggleable states	
emptyActionFunction(ToggleAntiwarp);	// TODO: should these be toggleable or have on/off states?
emptyActionFunction(ToggleCloak);
emptyActionFunction(ToggleMultifire);
emptyActionFunction(ToggleStealth);
emptyActionFunction(ToggleXRadar);

// misc
emptyActionFunction(Attach);	// turret attachment
emptyActionFunction(Warp);		

// menu commands
emptyActionFunction(Quit);
//emptyActionFunction(ShipChange);
emptyActionFunction(ToggleHelp);
emptyActionFunction(ToggleInfoBox);
emptyActionFunction(ToggleMenu);
emptyActionFunction(ToggleRadar);


/*
void SubspaceUserActionReceiver::doScrollUp(){}
void SubspaceUserActionReceiver::doScrollDown(){}
void SubspaceUserActionReceiver::doPageUp(){}
void SubspaceUserActionReceiver::doPageDown(){}

// player weapon commands
void SubspaceUserActionReceiver::doFireBrick(){}
void SubspaceUserActionReceiver::doFireBomb(){}
void SubspaceUserActionReceiver::doFireBullet(){}
void SubspaceUserActionReceiver::doFireBurst(){}
void SubspaceUserActionReceiver::doFireDecoy(){}
void SubspaceUserActionReceiver::doFireMine(){}
void SubspaceUserActionReceiver::doFirePortal(){}
void SubspaceUserActionReceiver::doFireRepel(){}
void SubspaceUserActionReceiver::doFireRocket(){}
void SubspaceUserActionReceiver::doFireThor(){}

// player movement commands
void SubspaceUserActionReceiver::doMoveForward(){}
void SubspaceUserActionReceiver::doMoveBackward(){}
void SubspaceUserActionReceiver::doTurnLeft(){}
void SubspaceUserActionReceiver::doTurnRight(){}
void SubspaceUserActionReceiver::doFastForward(){}		// fast are for afterburners / spectator
void SubspaceUserActionReceiver::doFastBackward(){}
void SubspaceUserActionReceiver::doFastLeft(){}
void SubspaceUserActionReceiver::doFastRight(){}

// player toggleable states	
void SubspaceUserActionReceiver::doToggleAntiwarp(){}	// TODO: should these be toggleable or have on/off states?
void SubspaceUserActionReceiver::doToggleCloak(){}
void SubspaceUserActionReceiver::doToggleMultifire(){}
void SubspaceUserActionReceiver::doToggleStealth(){}
void SubspaceUserActionReceiver::doToggleXRadar(){}

// misc
void SubspaceUserActionReceiver::doAttach(){}	// turret attachment
void SubspaceUserActionReceiver::doWarp(){}		

// menu commands
void SubspaceUserActionReceiver::doQuit(){}
//void SubspaceUserActionReceiver::doShipChange(){}
void SubspaceUserActionReceiver::doToggleHelp(){}
void SubspaceUserActionReceiver::doToggleInfoBox(){}
void SubspaceUserActionReceiver::doToggleMenu(){}
void SubspaceUserActionReceiver::doToggleRadar(){}
*/