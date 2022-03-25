//David Millman

#ifndef _SUBSPACEBULLET_H_
#define _SUBSPACEBULLET_H_

#include "SubspaceWeapon.h"
#include "TextureAnimated.h"

class SubspaceBullet :
	public SubspaceWeapon
{
public:
	SubspaceBullet(int level = 0, bool bouncing = false);
	~SubspaceBullet();
	
	WeaponData getWeaponSettings() const;

	void update(double timeMultiplier);

	int getLevel() const;
	virtual double getDamage() const;

	void setLevel(int level);		//level 0 = red guns
	void setBouncing(bool bouncing);

	virtual void setTexture();

	bool isBouncy() const;
	void destroy();

	void draw() const;

protected:

	bool bouncing_;
	int level_;

	TextureAnimated bulletTexture_;
};

#endif