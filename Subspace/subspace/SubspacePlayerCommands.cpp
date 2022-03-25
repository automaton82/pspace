#include "SubspacePlayerCommands.h"

#include "SubspacePlayerCommandReceiver.h"

namespace SubspacePlayerCommands
{

	void ChangeShip::execute(SubspacePlayerCommandReceiver* receiver)
	{
		receiver->doChangeShip(shipID);
	}

	void ChangeTeam::execute(SubspacePlayerCommandReceiver* receiver)
	{
		receiver->doChangeTeam(teamID);
	}

	///////////////////////////////////////
	#define basicPlayerCommandImpl(name)						\
	void name::execute(SubspacePlayerCommandReceiver* receiver)	\
	{															\
		receiver->do##name();									\
	}																	
	///////////////////////////////////////

	//weapons
	basicPlayerCommandImpl(FireBrick);
	basicPlayerCommandImpl(FireBomb);
	basicPlayerCommandImpl(FireBullet);
	basicPlayerCommandImpl(FireBurst);
	basicPlayerCommandImpl(FireDecoy);
	basicPlayerCommandImpl(FireMine);
	basicPlayerCommandImpl(FirePortal);
	basicPlayerCommandImpl(FireRepel);
	basicPlayerCommandImpl(FireRocket);
	basicPlayerCommandImpl(FireThor);

	//movement
	basicPlayerCommandImpl(MoveForward);
	basicPlayerCommandImpl(MoveBackward);
	basicPlayerCommandImpl(TurnLeft);
	basicPlayerCommandImpl(TurnRight);
	basicPlayerCommandImpl(FastForward);		//afterburner
	basicPlayerCommandImpl(FastBackward);
	basicPlayerCommandImpl(FastLeft);				
	basicPlayerCommandImpl(FastRight);

	//toggles
	basicPlayerCommandImpl(ToggleAntiwarp);
	basicPlayerCommandImpl(ToggleCloak);
    basicPlayerCommandImpl(ToggleMultifire);
	basicPlayerCommandImpl(ToggleStealth);
	basicPlayerCommandImpl(ToggleXRadar);

	//misc
	basicPlayerCommandImpl(Attach);	//turret attachment
	basicPlayerCommandImpl(Warp);	
}