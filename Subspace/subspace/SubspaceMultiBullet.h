//David Millman

#ifndef _SUBSPACEMULTIBULLET_H_
#define _SUBSPACEMULTIBULLET_H_

#include <list>
using std::list;

#include "SubspaceBullet.h"

//TODO: this whole thing with the zone is a big pile of spaghetti
class SubspaceMultiBullet :
	public SubspaceBullet
{
public:
	SubspaceMultiBullet(int level = 0, bool bouncing = false);
	~SubspaceMultiBullet();

	WeaponData getWeaponSettings() const;

	SubspaceMultiBullet* getParent() const;

	void setParent(SubspaceMultiBullet* parent);
	void addChild(SubspaceMultiBullet* child);
	void removeChild(SubspaceMultiBullet* child);

	void destroy();

	void destroyChildren(SubspaceMultiBullet* child);

private:

	SubspaceMultiBullet* parent_;
	
	typedef list<SubspaceMultiBullet*> MultiList;
	MultiList children_;
};

#endif