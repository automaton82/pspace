#include "SubspaceSound.h"

#include "SubspaceGlobal.h"

SoundManager subspaceSoundManager;
Sound subspaceSounds[SOUND_NumSounds];

void loadSubspaceSounds(const string& path)
{
	string zoneStr = "sound/";

	for(Uint i=0; i < SOUND_NumSounds; ++i)
	{
		subspaceSounds[i].setManager(&subspaceSoundManager);
	}

	subspaceSounds[SOUND_Alarm].load(zoneStr + "alarm.wa2");
	subspaceSounds[SOUND_Antiwarp].load(zoneStr + "antiwarp.wa2");
	subspaceSounds[SOUND_BallBounce].load(zoneStr + "ballbnc.wa2");
	subspaceSounds[SOUND_Bomb1].load(zoneStr + "bomb1.wa2");
	subspaceSounds[SOUND_Bomb2].load(zoneStr + "bomb2.wa2");
	subspaceSounds[SOUND_Bomb3].load(zoneStr + "bomb3.wa2");
	subspaceSounds[SOUND_Bomb4].load(zoneStr + "bomb4.wa2");
	subspaceSounds[SOUND_Bong1].load(zoneStr + "bong1.wa2");
	subspaceSounds[SOUND_Bong2].load(zoneStr + "bong2.wa2");
	subspaceSounds[SOUND_Bong3].load(zoneStr + "bong3.wa2");
	subspaceSounds[SOUND_Bong4].load(zoneStr + "bong4.wa2");
	subspaceSounds[SOUND_Bong5].load(zoneStr + "bong5.wa2");
	subspaceSounds[SOUND_Bong6].load(zoneStr + "bong6.wa2");
	subspaceSounds[SOUND_Bong7].load(zoneStr + "bong7.wa2");
	subspaceSounds[SOUND_Bong8].load(zoneStr + "bong8.wa2");
	subspaceSounds[SOUND_Bong9].load(zoneStr + "bong9.wa2");
	subspaceSounds[SOUND_Bong10].load(zoneStr + "bong10.wa2");
	subspaceSounds[SOUND_Bong11].load(zoneStr + "bong11.wa2");
	subspaceSounds[SOUND_Bong12].load(zoneStr + "bong12.wa2");
	subspaceSounds[SOUND_Bong13].load(zoneStr + "bong13.wa2");
	subspaceSounds[SOUND_Bong14].load(zoneStr + "bong14.wa2");
	subspaceSounds[SOUND_Bong15].load(zoneStr + "bong15.wa2");
	subspaceSounds[SOUND_Bong16].load(zoneStr + "bong16.wa2");
	subspaceSounds[SOUND_Bong17].load(zoneStr + "bong17.wa2");
	subspaceSounds[SOUND_Bong18].load(zoneStr + "bong18.wa2");
	subspaceSounds[SOUND_Bong19].load(zoneStr + "bong19.wa2");
	subspaceSounds[SOUND_Bong20].load(zoneStr + "bong20.wa2");
	subspaceSounds[SOUND_Bong21].load(zoneStr + "bong21.wa2");
	subspaceSounds[SOUND_Bong22].load(zoneStr + "bong22.wa2");
	subspaceSounds[SOUND_Bong23].load(zoneStr + "bong23.wa2");
	subspaceSounds[SOUND_Bong24].load(zoneStr + "bong24.wa2");
	subspaceSounds[SOUND_Bong25].load(zoneStr + "bong25.wa2");
	subspaceSounds[SOUND_Bong26].load(zoneStr + "bong26.wa2");
	subspaceSounds[SOUND_Bounce].load(zoneStr + "bong27.wa2");
	subspaceSounds[SOUND_Burst].load(zoneStr + "burst.wa2");
	subspaceSounds[SOUND_Catch].load(zoneStr + "catch.wa2");
	subspaceSounds[SOUND_Cloak].load(zoneStr + "cloak.wa2");
	subspaceSounds[SOUND_Decoy].load(zoneStr + "decoy.wa2");
	subspaceSounds[SOUND_EBomb1].load(zoneStr + "ebomb1.wa2");
	subspaceSounds[SOUND_EBomb2].load(zoneStr + "ebomb2.wa2");
	subspaceSounds[SOUND_EBomb3].load(zoneStr + "ebomb3.wa2");
	subspaceSounds[SOUND_EBomb4].load(zoneStr + "ebomb4.wa2");
	subspaceSounds[SOUND_EBombEx].load(zoneStr + "ebombex.wa2");
	subspaceSounds[SOUND_Explode0].load(zoneStr + "explode0.wa2");
	subspaceSounds[SOUND_Explode1].load(zoneStr + "explode1.wa2");
	subspaceSounds[SOUND_Explode2].load(zoneStr + "explode2.wa2");
	subspaceSounds[SOUND_Flag].load(zoneStr + "flag.wa2");
	subspaceSounds[SOUND_Goal].load(zoneStr + "goal.wa2");
	subspaceSounds[SOUND_Gun1].load(zoneStr + "gun1.wa2");
	subspaceSounds[SOUND_Gun2].load(zoneStr + "gun2.wa2");
	subspaceSounds[SOUND_Gun3].load(zoneStr + "gun3.wa2");
	subspaceSounds[SOUND_Gun4].load(zoneStr + "gun4.wa2");

	subspaceSounds[SOUND_Hum].load(zoneStr + "hum.wa2");
	
	subspaceSounds[SOUND_Mine1].load(zoneStr + "mine1.wa2");
	subspaceSounds[SOUND_Mine2].load(zoneStr + "mine2.wa2");
	subspaceSounds[SOUND_Mine3].load(zoneStr + "mine3.wa2");
	subspaceSounds[SOUND_Mine4].load(zoneStr + "mine4.wa2");
	subspaceSounds[SOUND_MultiOff].load(zoneStr + "multioff.wa2");
	subspaceSounds[SOUND_MultiOn].load(zoneStr + "multion.wa2");
	subspaceSounds[SOUND_Off].load(zoneStr + "off.wa2");
	subspaceSounds[SOUND_Prize].load(zoneStr + "prize.wa2");
	subspaceSounds[SOUND_Repel].load(zoneStr + "repel.wa2");
	subspaceSounds[SOUND_Rev].load(zoneStr + "rev.wa2");				//thruster
	subspaceSounds[SOUND_Rocket1].load(zoneStr + "rocket1.wa2");
	subspaceSounds[SOUND_Rocket2].load(zoneStr + "rocket2.wa2");
	subspaceSounds[SOUND_Stealth].load(zoneStr + "stealth.wa2");
	subspaceSounds[SOUND_Thor].load(zoneStr + "thor.wa2");
	subspaceSounds[SOUND_Throw].load(zoneStr + "throw.wa2");
	subspaceSounds[SOUND_Thrust].load(zoneStr + "thrust.wa2");
	subspaceSounds[SOUND_Victory].load(zoneStr + "victory.wa2");
	subspaceSounds[SOUND_Victory1].load(zoneStr + "victory1.wa2");
	subspaceSounds[SOUND_Wall].load(zoneStr + "wall.wa2");
	subspaceSounds[SOUND_Warp].load(zoneStr + "warp.wa2");
	subspaceSounds[SOUND_WarpPoint].load(zoneStr + "warppnt.wa2");
	subspaceSounds[SOUND_XRadar].load(zoneStr + "xradar.wa2");
}