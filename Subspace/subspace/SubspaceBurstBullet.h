//David Millman

#ifndef _SUBSPACEBURSTBULLET_H_
#define _SUBSPACEBURSTBULLET_H_

#include "SubspaceBullet.h"

class SubspaceBurstBullet :
	public SubspaceBullet
{
public:
	SubspaceBurstBullet();
	~SubspaceBurstBullet();

	double getDamage() const;

	WeaponData getWeaponSettings() const;

	bool isActive() const;

	void update(double time);		//TODO: create "bounced" function instead? - might be good

private:

	bool isActive_;
};

#endif