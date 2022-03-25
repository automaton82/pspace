#include "SubspaceMultiBullet.h"

#include "SubspaceZone.h"

#include "MathUtil.h"
using namespace MathUtil;

SubspaceMultiBullet::SubspaceMultiBullet(int level, bool bouncing) : SubspaceBullet(level, bouncing),
	parent_(0)
{
}

SubspaceMultiBullet::~SubspaceMultiBullet()
{
}

WeaponData SubspaceMultiBullet::getWeaponSettings() const
{
	WeaponData w = SubspaceBullet::getWeaponSettings();

	w.alternate = 1;
	
	return w;
}

SubspaceMultiBullet* SubspaceMultiBullet::getParent() const
{
	return parent_;
}

void SubspaceMultiBullet::setParent(SubspaceMultiBullet* parent)
{
	parent_ = parent;
}

void SubspaceMultiBullet::addChild(SubspaceMultiBullet* child)
{
	children_.push_back(child);
}

void SubspaceMultiBullet::removeChild(SubspaceMultiBullet* child)
{
	children_.remove(child);
}

void SubspaceMultiBullet::destroy()
{
	if(!isGarbage())
	{
		if(parent_)
		{
			parent_->removeChild(this);
			parent_->destroy();
		}
		else
		{
			destroyChildren(0);
		}

		zone_->destroyBullet(this);
	}
}

void SubspaceMultiBullet::destroyChildren(SubspaceMultiBullet* child)
{
	MultiList::iterator i;
	for(i = children_.begin(); i != children_.end(); ++i)
	{
		SubspaceMultiBullet* bullet = (*i);
		
		if(bullet != child)
		{
			bullet->setParent(0);
			bullet->destroy();
		}
	}
	children_.clear();
}