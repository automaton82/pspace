//David Millman

#ifndef _SUBSPACEPRIZEGENERATOR_H_
#define _SUBSPACEPRIZEGENERATOR_H_

#include "PrizeSettings.h"
#include "SubspacePRNG.h"

#include "SubspacePrize.h"

class SubspacePrizeGenerator
{
public:
	SubspacePrizeGenerator();
	~SubspacePrizeGenerator();

	const PrizeSettings& getPrizeWeights() const;
	Uint getPrizeWeightTotal() const;

	void setSeed(Uint32 seed);
	void setPopulation(Uint pop);
	void setPrizeWeights(const PrizeSettings& settings, Uint16 prizeNegativeFactor = 0);
	void setSpawnRange(Uint range, Uint rangeUpgrade);
	void setPrizeTimeToLive(double min, double max);	//in milliseconds

	SubspacePrize* createPrize();

	PrizeType getPrize();
	PrizeType getMultiPrize();
	
private:

	void calcPrizeWeightTotal();

private:

	SS_HEAVY_PRNG prng_;
	Uint32 seed_;

	Uint population_;

	//regular prizes
	PrizeSettings prizeWeights_;
	Uint prizeWeightTotal_, prizeNegativeFactor_;

	//multi prize
	PrizeSettings multiPrizeWeights_;
	Uint multiPrizeWeightTotal_;

	Uint spawnRange_, spawnRangeUpgrade_;
	double timeToLiveMin_, timeToLiveMax_;

	//calculation only
	/*double buckets[sizeof(PrizeSettings)];	
	double bucketSum;*/
};

#endif