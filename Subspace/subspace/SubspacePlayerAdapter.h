#ifndef _SUBSPACEPLAYERADAPTER_H_
#define _SUBSPACEPLAYERADAPTER_H_

#include "SubspacePlayerCommandReceiver.h"

class SubspaceZone;

// this class connects client player commands to the zone
// TODO: this should handle all player/client related notions, e.g. koth timers, position-sending thread
class SubspacePlayerAdapter : 
	public SubspacePlayerCommandReceiver
{
public:

	SubspacePlayerAdapter();

	// Mutators
	void setZone(SubspaceZone* zone);

	///////////////////////////////////

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

	//movement
	virtual void doMoveForward();
	virtual void doMoveBackward();
	virtual void doTurnLeft();
	virtual void doTurnRight();
	virtual void doFastForward();		//afterburner
	virtual void doFastBackward();
	virtual void doFastLeft();				
	virtual void doFastRight();

	//toggles
	virtual void doToggleAntiwarp();
	virtual void doToggleCloak();
    virtual void doToggleMultifire();
	virtual void doToggleStealth();
	virtual void doToggleXRadar();

	//misc
	virtual void doAttach();	//turret attachment
	virtual void doWarp();

	//ship changes
	virtual void doChangeShip(int shipID);
	virtual void doTeamChange(int teamID);

private:

	SubspaceZone* zone_;
};

#endif