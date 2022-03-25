#ifndef _WEAPONSETTINGS_H_
#define _WEAPONSETTINGS_H_

#include "DataTypes.h"

enum WeaponType
{
	WEP_None,
	WEP_Bullet,
	WEP_BouncingBullet,
	WEP_Bomb,
	WEP_ProximityBomb,
	WEP_Repel,
	WEP_Decoy,
	WEP_Burst,
	WEP_Thor,
	WEP_Unknown
};

union WeaponData
{
	struct
	{
		unsigned type		: 5;	// enum Projectile_Types
		unsigned level		: 2;	// Only for bombs/bullets
		unsigned bouncing	: 1;	// Only for bombs
		unsigned emp		: 1;	// Only for bombs
		unsigned isBomb		: 1;	// ? This is a bomb
		unsigned shrapCount	: 5;	// 0-31
		unsigned alternate	: 1;	// Bombs -> Mines, Bullets -> Multifire
	};

	Uint16 u;
};

#endif