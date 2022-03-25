#ifndef _SUBSPACEEVENT_H_
#define _SUBSPACEEVENT_H_


enum SubspaceEventType
{
	EVENT_Unknown = 0,

	EVENT_TimeTick,

	//chat
	EVENT_ScrollUp,
	EVENT_ScrollDown,
	EVENT_ScrollUpFast,
	EVENT_ScrollDownFast,

	//weapons
	EVENT_FireBrick,
	EVENT_FireBomb,
	EVENT_FireBullet,
	EVENT_FireBurst,
	EVENT_FireDecoy,
	EVENT_FireMine,
	EVENT_FirePortal,
	EVENT_FireRepel,
	EVENT_FireRocket,
	EVENT_FireThor,

	//movement
	EVENT_MoveForward,
	EVENT_MoveBackward,
	EVENT_TurnLeft,
	EVENT_TurnRight,
	EVENT_FastForward,		//afterburner
	EVENT_FastBackward,
	EVENT_FastLeft,				
	EVENT_FastRight,

	//toggles
	EVENT_ToggleAntiwarp,
	EVENT_ToggleCloak,
    EVENT_ToggleMultifire,
	EVENT_ToggleStealth,
	EVENT_ToggleXRadar,

	//misc
	EVENT_Attach,	//turret attachment
	EVENT_Warp,

	//menu commands
	EVENT_ShipChange,
	EVENT_ToggleHelp,		//TODO: turn these into a chat command
	EVENT_ToggleInfoBox,
	EVENT_ToggleMenu,
	EVENT_ToggleRadar,
	EVENT_ToggleBigRadar,
	EVENT_Quit,

	EVENT_NumEvents
};

struct SubspaceEvent
{
public:
	SubspaceEvent();
	SubspaceEvent(SubspaceEventType type, void* param);

	SubspaceEventType type;
	void* param;
};

#endif