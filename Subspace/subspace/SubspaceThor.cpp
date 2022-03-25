#include "SubspaceThor.h"

#include "TextureManager.h"

#include "SubspaceZone.h"

SubspaceThor::SubspaceThor() : SubspaceBomb(5, 0, 0, false, 0)
{
}

SubspaceThor::~SubspaceThor()
{
}

WeaponData SubspaceThor::getWeaponSettings() const
{
	WeaponData w;
	w.u = 0;
	w.bouncing = 0;
	w.emp = 0;
	w.shrapCount = 0;
	w.isBomb = 1;
	w.level = 5-1;
	w.type = WEP_Thor;
		
	return w;
}

void SubspaceThor::update(double time)
{
	timeToLive_ -= time;

	if(timeToLive_ <= 0)
		this->garbage = true;

							//ignore map collisions
	SubspaceGameObject::update(time);

	bombTexture_.advanceFrame(time);
}
