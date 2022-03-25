#ifndef _SUBSPACESOUND_H_
#define _SUBSPACESOUND_H_

#include <string>
using std::string;

#include "Sound.h"
#include "SoundManager.h"

extern SoundManager subspaceSoundManager;

enum SoundType
{
	SOUND_Alarm,
	SOUND_Antiwarp,
	SOUND_BallBounce,
	SOUND_Bomb1,
	SOUND_Bomb2,
	SOUND_Bomb3,
	SOUND_Bomb4,
	SOUND_Bong1,
	SOUND_Bong2,
	SOUND_Bong3,
	SOUND_Bong4,
	SOUND_Bong5,
	SOUND_Bong6,
	SOUND_Bong7,
	SOUND_Bong8,
	SOUND_Bong9,
	SOUND_Bong10,
	SOUND_Bong11,
	SOUND_Bong12,
	SOUND_Bong13,
	SOUND_Bong14,
	SOUND_Bong15,
	SOUND_Bong16,
	SOUND_Bong17,
	SOUND_Bong18,
	SOUND_Bong19,
	SOUND_Bong20,
	SOUND_Bong21,
	SOUND_Bong22,
	SOUND_Bong23,
	SOUND_Bong24,
	SOUND_Bong25,
	SOUND_Bong26,
	SOUND_Bounce,
	SOUND_Burst,
	SOUND_Catch,
	SOUND_Cloak,
	SOUND_Decoy,
	SOUND_EBomb1,
	SOUND_EBomb2,
	SOUND_EBomb3,
	SOUND_EBomb4,
	SOUND_EBombEx,
	SOUND_Explode0,
	SOUND_Explode1,
	SOUND_Explode2,
	SOUND_Flag,
	SOUND_Goal,
	SOUND_Gun1,
	SOUND_Gun2,
	SOUND_Gun3,
	SOUND_Gun4,
	SOUND_Hum,
	SOUND_Mine1,
	SOUND_Mine2,
	SOUND_Mine3,
	SOUND_Mine4,
	SOUND_MultiOff,
	SOUND_MultiOn,
	SOUND_Off,
	SOUND_Prize,
	SOUND_Repel,
	SOUND_Rev,		//thruster
	SOUND_Rocket1,	//rocket w/ explosion
	SOUND_Rocket2,	//rocket repeat sound
	SOUND_Stealth,
	SOUND_Thor,
	SOUND_Throw,
	SOUND_Thrust,	//afterburner thruster
	SOUND_Victory,
	SOUND_Victory1,
	SOUND_Wall,
	SOUND_Warp,
	SOUND_WarpPoint,
	SOUND_XRadar,

	SOUND_NumSounds
};

extern Sound subspaceSounds[SOUND_NumSounds];

void loadSubspaceSounds(const string& path);

#endif