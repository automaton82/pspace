//David Millman

#ifndef _SUBSPACESHRAPNEL_H_
#define _SUBSPACESHRAPNEL_H_

#include "SubspaceBullet.h"

class SubspaceShrapnel :
	public SubspaceBullet
{
public:
	SubspaceShrapnel(int level = 0, bool bouncing = false);
	~SubspaceShrapnel();

	//no weapon settings

	double getDamage() const;
	void setTexture();

	bool isActive() const;
	bool isBouncy() const;

	void destroy();

	void update(double time);		//TODO: create "bounced" function instead? - might be good

	static const double inactiveTime;	//inactive for first quarter second of life

private:

	double timeAlive_;
};

#endif