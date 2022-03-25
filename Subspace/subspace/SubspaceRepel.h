//David Millman

#ifndef _SUBSPACEREPEL_H_
#define _SUBSPACEREPEL_H_

#include "SubspaceWeapon.h"
#include "TextureAnimated.h"

class SubspaceRepel :
	public SubspaceWeapon
{
public:
	SubspaceRepel();
	~SubspaceRepel();

	WeaponData getWeaponSettings() const;
	
	void addTarget(SubspaceGameObject* target);		//target affected by repel
	void updateTargets();

	void update(double timeMultiplier);

	void draw() const;

private:
	struct targetData
	{
		Vector dir;
		SubspaceGameObject* object;
	};
	typedef list<targetData> TargetList;
	TargetList targets_;

	bool doDisplay_;
	TextureAnimated repelTexture_;
};

#endif