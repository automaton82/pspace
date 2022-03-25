#include "SubspacePlayer.h"

#include "SubspaceSound.h"

void SubspacePlayer::toggleAntiwarp()
{
	if(!items_.antiwarp)
		return;
	
	status_.antiwarp = !status_.antiwarp;
	
	if(isMyPlayer())
	{
		if(status_.antiwarp)	//turned on
			subspaceSounds[SOUND_Antiwarp].play();
		else					//turned off
			subspaceSounds[SOUND_Off].play();
	}
}

void SubspacePlayer::toggleCloak()
{
	if(!items_.cloak)
		return;
	
	status_.cloak = !status_.cloak;

	if(isMyPlayer())
	{
		if(status_.cloak)	//turned on
			subspaceSounds[SOUND_Cloak].play();
		else					//turned off
			subspaceSounds[SOUND_Off].play();
	}
}

void SubspacePlayer::toggleMultifire()
{
	if(!items_.multifire)
		return;

	weapons_.guns.multifire = !weapons_.guns.multifire;

	if(isMyPlayer())
	{
		if(weapons_.guns.multifire)	//turned on
			subspaceSounds[SOUND_MultiOn].play();
		else					//turned off
			subspaceSounds[SOUND_MultiOff].play();
	}
}

void SubspacePlayer::toggleStealth()
{
	if(!items_.stealth)
		return;

	status_.stealth = !status_.stealth;

	if(isMyPlayer())
	{
		if(status_.stealth)	//turned on
			subspaceSounds[SOUND_Stealth].play();
		else					//turned off
			subspaceSounds[SOUND_Off].play();
	}
}

void SubspacePlayer::toggleXRadar()
{
	if(!items_.xRadar)
		return;

	status_.xRadar = !status_.xRadar;

	if(isMyPlayer())
	{
		if(status_.xRadar)	//turned on
			subspaceSounds[SOUND_XRadar].play();
		else					//turned off
			subspaceSounds[SOUND_Off].play();
	}
}

void SubspacePlayer::setSafe(bool isSafe)
{
	if(status_.safe && isSafe)	//still safe
	{
		//do nothing
	}
	else if(isSafe)
		safeTime_ = arenaSettings_->SafetyLimit * 10.0;	//convert 10ms to ms
	else
		safeTime_ = 0;

	status_.safe = isSafe;
}

bool SubspacePlayer::isSafe() const
{
	return status_.safe;
}

double SubspacePlayer::getSafeTime() const
{
	return safeTime_;
}