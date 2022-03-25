#ifndef _SUBSPACEWEAPON_H_
#define _SUBSPACEWEAPON_H_

#include "SubspaceGameObject.h"

class SubspacePlayer;
class SubspaceZone;

#include "WeaponSettings.h"

class SubspaceWeapon :
	public SubspaceGameObject
{
public:
	SubspaceWeapon(SubspacePlayer* owner = 0);
	virtual ~SubspaceWeapon();
	
	void setOwner(SubspacePlayer* owner);
	SubspacePlayer* getOwner() const;

	void setZone(SubspaceZone* zone);
	SubspaceZone* getZone() const;

	WeaponType getWeaponType() const;
	virtual WeaponData getWeaponSettings() const;

	virtual bool isBouncy() const;
	virtual void destroy();				//event on destroy

	void setTimeToLive(double timeToLive);
	double getTimeToLive() const;

	virtual void update(double timeMultiplier);	//does collision detection

protected:

	static team_;

	double timeToLive_;

	SubspaceZone* zone_;
	SubspacePlayer* owner_;
	WeaponType weaponType;
};

#endif
