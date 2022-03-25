#include "SubspaceUserActions.h"

SubspaceUserAction::~SubspaceUserAction()
{
}

void SubspaceUserAction::execute(SubspaceUserActionReceiver* receiver)
{
}

namespace SubspaceUserActions
{
	///////////////////////////////////////
	#define basicActionImpl(name)								\
	void name::execute(SubspaceUserActionReceiver* receiver)	\
	{															\
		receiver->do##name();									\
	}															\		
	///////////////////////////////////////

	// chat/info box commands
	basicActionImpl(ScrollUp);   
	basicActionImpl(ScrollDown);   
	basicActionImpl(PageUp);   
	basicActionImpl(PageDown);   

	// player weapon commands
	basicActionImpl(FireBrick);   
	basicActionImpl(FireBomb);   
	basicActionImpl(FireBullet);   
	basicActionImpl(FireBurst);   
	basicActionImpl(FireDecoy);   
	basicActionImpl(FireMine);   
	basicActionImpl(FirePortal);   
	basicActionImpl(FireRepel);   
	basicActionImpl(FireRocket);   
	basicActionImpl(FireThor);   

	// player movement commands
	basicActionImpl(MoveForward);   
	basicActionImpl(MoveBackward);   
	basicActionImpl(TurnLeft);   
	basicActionImpl(TurnRight);   
	basicActionImpl(FastForward);   		// fast are for afterburners / spectator
	basicActionImpl(FastBackward);   
	basicActionImpl(FastLeft);   
	basicActionImpl(FastRight);   

	// player toggleable states	
	basicActionImpl(ToggleAntiwarp);   	// TODO: should these be toggleable or have on/off states?
	basicActionImpl(ToggleCloak);   
	basicActionImpl(ToggleMultifire);   
	basicActionImpl(ToggleStealth);   
	basicActionImpl(ToggleXRadar);   

	// misc
	basicActionImpl(Attach);   	// turret attachment
    basicActionImpl(Warp);   		

	// menu commands
	basicActionImpl(Quit);   
	//basicActionImpl(ShipChange);   
	//basicActionImpl(ToggleHelp);   
	//basicActionImpl(ToggleInfoBox);   
	//basicActionImpl(ToggleMenu);   
	//basicActionImpl(ToggleRadar);   
}