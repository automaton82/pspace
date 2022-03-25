#ifndef _SUBSPACEZONECOMMANDS_H_
#define _SUBSPACEZONECOMMANDS_H_

class SubspaceZoneCommandReceiver;

class SubspaceZoneCommand
{
public:
	virtual ~SubspaceZoneCommand() {}

	virtual void execute(SubspaceZoneCommandReceiver* listener) = 0;
};

namespace SubspaceZoneCommands
{
	// NOTE: these actions intentionally have no parameters, since any user input is based on one key
	///////////////////////////////////////
	#define basicCommand(name)												\
		class name															\
			: public SubspaceZoneCommand									\
		{																	\
			public: void execute(SubspaceZoneCommandReceiver* receiver)	{}	\
		}				
	///////////////////////////////////////

	//weapons
	basicCommand(FireBrick);
	basicCommand(FireBomb);
	basicCommand(FireBullet);
	basicCommand(FireBurst);
	basicCommand(FireDecoy);
	basicCommand(FireMine);
	basicCommand(FirePortal);
	basicCommand(FireRepel);
	basicCommand(FireRocket);
	basicCommand(FireThor);

	//movement
	basicCommand(MoveForward);
	basicCommand(MoveBackward);
	basicCommand(TurnLeft);
	basicCommand(TurnRight);
	basicCommand(FastForward);		//afterburner
	basicCommand(FastBackward);
	basicCommand(FastLeft);				
	basicCommand(FastRight);

	//toggles
	basicCommand(ToggleAntiwarp);
	basicCommand(ToggleCloak);
    basicCommand(ToggleMultifire);
	basicCommand(ToggleStealth);
	basicCommand(ToggleXRadar);

	//misc
	basicCommand(Attach);	//turret attachment
	basicCommand(Warp);
}

#endif