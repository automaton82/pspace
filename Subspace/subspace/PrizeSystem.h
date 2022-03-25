#ifndef _PRIZE_H_
#define _PRIZE_H_

#include "datatypes.h"
#include "prng.h"
#include "settings.h"

const Uint32 cosmicPrize = 0x10624DD3;

struct prize
{
	Uint16 x, y;
	Sint16 type;
	Uint32 time;
};

class PrizeSystem
{	
	Uint32 rlevel;
	SS_HEAVY_PRNG prng;

	Uint32 lastTime;

	Uint32 prizeWeightTotal;
	Uint16 prizeFactor;
	Uint16 prizeDelay;
	Uint16 prizeHideCount;
	Uint16 prizeUpgradeVirtual;
	Uint16 prizeMinimumVirtual;
	Uint16 prizeMaxExist;
	Uint16 prizeMinExist;
	Uint16 prizeNegativeFactor;
	prizeSettings prizeWeights;

	Uint32 getNumberOfGreens(Uint16 population);
	Prize *createGreen(Uint16 population);

public:
	prizeSystem();

	void setSettings(arenaSettings *s, BYTE *m);
	void randomize(Uint32 seed, Uint32 time);
	void doPrizes(Uint16 population);
	void clearList();
	void killGreen(Uint32 time, Uint16 x, Uint16 y);
};

#endif	// PRIZE_H
