#ifndef _SUBSPACEUSERACTIONS_H_
#define _SUBSPACEUSERACTIONS_H_

#include "DataTypes.h"

class SubspaceUserActionReceiver;

class SubspaceUserAction
{
public:
	virtual ~SubspaceUserAction();

	virtual void execute(SubspaceUserActionReceiver* receiver) = 0;
};

namespace SubspaceUserActions
{
	// NOTE: these actions intentionally have no parameters, since any user input is based on one key
	///////////////////////////////////////
	#define basicAction(name)												\
		class name															\
			: public SubspaceUserAction										\
		{																	\
			public: void execute(SubspaceUserActionReceiver* receiver)		\
		}				
	///////////////////////////////////////

	// chat/info box commands
	basicAction(ScrollUp);   
	basicAction(ScrollDown);   
	basicAction(PageUp);   
	basicAction(PageDown); 
	basicAction(SendChat);

	// player weapon commands
	basicAction(FireBrick);   
	basicAction(FireBomb);   
	basicAction(FireBullet);   
	basicAction(FireBurst);   
	basicAction(FireDecoy);   
	basicAction(FireMine);   
	basicAction(FirePortal);   
	basicAction(FireRepel);   
	basicAction(FireRocket);   
	basicAction(FireThor);   

	// player movement commands
	basicAction(MoveForward);   
	basicAction(MoveBackward);   
	basicAction(TurnLeft);   
	basicAction(TurnRight);   
	basicAction(FastForward);   		// fast are for afterburners / spectator
	basicAction(FastBackward);   
	basicAction(FastLeft);   
	basicAction(FastRight);   

	// player toggleable states	
	basicAction(ToggleAntiwarp);   	// TODO: should these be toggleable or have on/off states?
	basicAction(ToggleCloak);   
	basicAction(ToggleMultifire);   
	basicAction(ToggleStealth);   
	basicAction(ToggleXRadar);   

	// misc
	basicAction(Attach);   	// turret attachment
    basicAction(Warp);   		

	// menu commands
	//basicAction(Quit);   
	//basicAction(ShipChange);   
	//basicAction(ToggleHelp);   
	//basicAction(ToggleInfoBox);   
	//basicAction(ToggleMenu);   
	//basicAction(ToggleRadar);   

	/*
	// chat/info box commands
	class ScrollUp		{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class ScrollDown	{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class PageUp		{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class PageDown		{ public: void execute(SubspaceUserActionReceiver* receiver) };

	// player weapon commands
	class FireBrick		{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class FireBomb		{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class FireBullet	{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class FireBurst		{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class FireDecoy		{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class FireMine		{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class FirePortal	{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class FireRepel		{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class FireRocket	{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class FireThor		{ public: void execute(SubspaceUserActionReceiver* receiver) };

	// player movement commands
	class MoveForward	{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class MoveBackward	{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class TurnLeft		{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class TurnRight		{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class FastForward	{ public: void execute(SubspaceUserActionReceiver* receiver) };		// fast are for afterburners / spectator
	class FastBackward	{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class FastLeft		{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class FastRight		{ public: void execute(SubspaceUserActionReceiver* receiver) };

	// player toggleable states	
	class ToggleAntiwarp	{ public: void execute(SubspaceUserActionReceiver* receiver) };	// TODO: should these be toggleable or have on/off states?
	class ToggleCloak		{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class ToggleMultifire	{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class ToggleStealth		{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class ToggleXRadar		{ public: void execute(SubspaceUserActionReceiver* receiver) };

	// misc
	class Attach	{ public: void execute(SubspaceUserActionReceiver* receiver) };	// turret attachment
    class Warp		{ public: void execute(SubspaceUserActionReceiver* receiver) };		

	// menu commands
	class Quit			{ public: void execute(SubspaceUserActionReceiver* receiver) };
	//class ShipChange{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class ToggleHelp	{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class ToggleInfoBox	{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class ToggleMenu	{ public: void execute(SubspaceUserActionReceiver* receiver) };
	class ToggleRadar	{ public: void execute(SubspaceUserActionReceiver* receiver) };
	*/
}

#endif