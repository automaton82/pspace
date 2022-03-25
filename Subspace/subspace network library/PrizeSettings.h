#ifndef _PRIZESETTINGS_H_
#define _PRIZESETTINGS_H_

#include "DataTypes.h"

// Wrapper for raw prize type numbers

enum PrizeType
{
	PRIZE_Unknown,		// 0
	PRIZE_Recharge,
	PRIZE_Energy,
	PRIZE_Rotation,
	PRIZE_Stealth,
	PRIZE_Cloak,
	PRIZE_XRadar,
	PRIZE_Warp,
	PRIZE_Guns,
	PRIZE_Bombs,
	PRIZE_BBullets,
	PRIZE_Thruster,
	PRIZE_TopSpeed,
	PRIZE_FullCharge,
	PRIZE_EngineShutdown,
	PRIZE_Multifire,
	PRIZE_Proximity,
	PRIZE_Super,
	PRIZE_Shields,
	PRIZE_Shrapnel,
	PRIZE_Antiwarp,
	PRIZE_Repel,
	PRIZE_Burst,
	PRIZE_Decoy,
	PRIZE_Thor,
	PRIZE_Multiprize,
	PRIZE_Brick,
	PRIZE_Rocket,
	PRIZE_Portal,		// 28

	PRIZE_NumPrizes
};

// Prize settings

struct PrizeSettings				// 28 bytes wide
{	// All Snrrrub
	BYTE Recharge;					// 1400 [fixed] PrizeWeight:QuickCharge:::Likelyhood of 'Recharge' prize appearing
	BYTE Energy;					// 1401 PrizeWeight:Energy:::Likelyhood of 'Energy Upgrade' prize appearing
	BYTE Rotation;					// 1402 PrizeWeight:Rotation:::Likelyhood of 'Rotation' prize appearing
	BYTE Stealth;					// 1403 PrizeWeight:Stealth:::Likelyhood of 'Stealth' prize appearing
	BYTE Cloak;						// 1404 PrizeWeight:Cloak:::Likelyhood of 'Cloak' prize appearing
	BYTE XRadar;					// 1405 PrizeWeight:XRadar:::Likelyhood of 'XRadar' prize appearing
	BYTE Warp;						// 1406 PrizeWeight:Warp:::Likelyhood of 'Warp' prize appearing
	BYTE Gun;						// 1407 PrizeWeight:Gun:::Likelyhood of 'Gun Upgrade' prize appearing
	BYTE Bomb;						// 1408 PrizeWeight:Bomb:::Likelyhood of 'Bomb Upgrade' prize appearing
	BYTE BouncingBullets;			// 1409 PrizeWeight:BouncingBullets:::Likelyhood of 'Bouncing Bullets' prize appearing
	BYTE Thruster;					// 1410 PrizeWeight:Thruster:::Likelyhood of 'Thruster' prize appearing
	BYTE TopSpeed;					// 1411 PrizeWeight:TopSpeed:::Likelyhood of 'Speed' prize appearing
	BYTE QuickCharge;				// 1412 [fixed] PrizeWeight:Recharge:::Likelyhood of 'Full Charge' prize appearing (NOTE! This is FULL CHARGE, not Recharge!! stupid vie)
	BYTE Glue;						// 1413 PrizeWeight:Glue:::Likelyhood of 'Engine Shutdown' prize appearing
	BYTE MultiFire;					// 1414 PrizeWeight:MultiFire:::Likelyhood of 'MultiFire' prize appearing
	BYTE Proximity;					// 1415 PrizeWeight:Proximity:::Likelyhood of 'Proximity Bomb' prize appearing
	BYTE AllWeapons;				// 1416 PrizeWeight:AllWeapons:::Likelyhood of 'Super!' prize appearing
	BYTE Shields;					// 1417 PrizeWeight:Shields:::Likelyhood of 'Shields' prize appearing
	BYTE Shrapnel;					// 1418 PrizeWeight:Shrapnel:::Likelyhood of 'Shrapnel Upgrade' prize appearing
	BYTE AntiWarp;					// 1419 PrizeWeight:AntiWarp:::Likelyhood of 'AntiWarp' prize appearing
	BYTE Repel;						// 1420 PrizeWeight:Repel:::Likelyhood of 'Repel' prize appearing
	BYTE Burst;						// 1421 PrizeWeight:Burst:::Likelyhood of 'Burst' prize appearing
	BYTE Decoy;						// 1422 PrizeWeight:Decoy:::Likelyhood of 'Decoy' prize appearing
	BYTE Thor;						// 1423 PrizeWeight:Thor:::Likelyhood of 'Thor' prize appearing
	BYTE MultiPrize;				// 1424 PrizeWeight:MultiPrize:::Likelyhood of 'Multi-Prize' prize appearing
	BYTE Brick;						// 1425 PrizeWeight:Brick:::Likelyhood of 'Brick' prize appearing
	BYTE Rocket;					// 1426 PrizeWeight:Rocket:::Likelyhood of 'Rocket' prize appearing
	BYTE Portal;					// 1427 PrizeWeight:Portal:::Likelyhood of 'Portal' prize appearing

	Uint getTotalWeight() const
	{
		Byte* weight = (Byte*)this;
		Uint total = 0;
		for(Uint i=0; i < sizeof(PrizeSettings); ++i)
		{
			total += *weight++;
		}

		return total;
	}
};

#endif