#ifndef _SUBSPACEPLAYERCOMMANDS_H_
#define _SUBSPACEPLAYERCOMMANDS_H_

class SubspacePlayerCommandReceiver;

class SubspacePlayerCommand
{
public:
	virtual ~SubspacePlayerCommand() {}

	virtual void execute(SubspacePlayerCommandReceiver* listener) = 0;
};

namespace SubspacePlayerCommands
{
	class ChangeShip : 
		public SubspacePlayerCommand
	{
	public:
		void execute(SubspacePlayerCommandReceiver* receiver);
		
		int shipID;
	};

	class ChangeTeam : 
		public SubspacePlayerCommand
	{
	public:
		void execute(SubspacePlayerCommandReceiver* receiver);
		
		int teamID;
	};
	
	// NOTE: these actions intentionally have no parameters, since any user input is based on one key
	///////////////////////////////////////
	#define basicPlayerCommand(name)										\
		class name															\
			: public SubspacePlayerCommand									\
		{																	\
		public:	void execute(SubspacePlayerCommandReceiver* receiver);		\
		}				
	///////////////////////////////////////

	//weapons
	basicPlayerCommand(FireBrick);
	basicPlayerCommand(FireBomb);
	basicPlayerCommand(FireBullet);
	basicPlayerCommand(FireBurst);
	basicPlayerCommand(FireDecoy);
	basicPlayerCommand(FireMine);
	basicPlayerCommand(FirePortal);
	basicPlayerCommand(FireRepel);
	basicPlayerCommand(FireRocket);
	basicPlayerCommand(FireThor);

	//movement
	basicPlayerCommand(MoveForward);
	basicPlayerCommand(MoveBackward);
	basicPlayerCommand(TurnLeft);
	basicPlayerCommand(TurnRight);
	basicPlayerCommand(FastForward);		//afterburner
	basicPlayerCommand(FastBackward);
	basicPlayerCommand(FastLeft);				
	basicPlayerCommand(FastRight);

	//toggles
	basicPlayerCommand(ToggleAntiwarp);
	basicPlayerCommand(ToggleCloak);
    basicPlayerCommand(ToggleMultifire);
	basicPlayerCommand(ToggleStealth);
	basicPlayerCommand(ToggleXRadar);

	//misc
	basicPlayerCommand(Attach);	//turret attachment
	basicPlayerCommand(Warp);
	
	//ship changes
	/*basicPlayerCommand(ShipWarbird);
	basicPlayerCommand(ShipJavelin);
	basicPlayerCommand(ShipSpider);
	basicPlayerCommand(ShipLeviathan);
	basicPlayerCommand(ShipTerrier);
	basicPlayerCommand(ShipWeasel);
	basicPlayerCommand(ShipLancaster);
	basicPlayerCommand(ShipShark);*/
}

#endif