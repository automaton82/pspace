#include "SubspacePrizeGenerator.h"

#include "SubspaceUtil.h"

SubspacePrizeGenerator::SubspacePrizeGenerator()
{
}

SubspacePrizeGenerator::~SubspacePrizeGenerator()
{
}

const PrizeSettings& SubspacePrizeGenerator::getPrizeWeights() const
{
	return prizeWeights_;
}

Uint SubspacePrizeGenerator::getPrizeWeightTotal() const
{
	return prizeWeightTotal_;
}

void SubspacePrizeGenerator::setSeed(Uint32 seed)
{
	seed_ = seed;
	prng_.seed(seed_);
}

void SubspacePrizeGenerator::setPopulation(Uint pop)
{
	population_ = pop;
}

void SubspacePrizeGenerator::setPrizeWeights(const PrizeSettings& settings, Uint16 prizeNegativeFactor)
{
	memcpy(&prizeWeights_, &settings, sizeof(PrizeSettings));

	//calcPrizeWeightTotal();
	prizeNegativeFactor_ = max(prizeNegativeFactor, 1);
	prizeWeightTotal_ = prizeWeights_.getTotalWeight();

	memcpy(&multiPrizeWeights_, &settings, sizeof(PrizeSettings));
	multiPrizeWeights_.AllWeapons = 0;
	multiPrizeWeights_.Brick = 0;
	multiPrizeWeights_.Glue = 0;
	multiPrizeWeights_.MultiPrize = 0;
	multiPrizeWeights_.Shields = 0;
	multiPrizeWeights_.Warp = 0;

	multiPrizeWeightTotal_ = multiPrizeWeights_.getTotalWeight();
}

/*void SubspacePrizeGenerator::calcPrizeWeightTotal()
{
	prizeWeightTotal_ = 0;
	Byte *pw = (Byte*)&prizeWeights_;
	for(int i = 0; i < sizeof(PrizeSettings); ++i)
	{
		prizeWeightTotal_ += (*pw++);
	}

	//create buckets

	bucketSum = 0;
	Byte* weight = (Byte*)&prizeWeights_;
	for(Uint i=0; i < sizeof(PrizeSettings); ++i)
	{
		Byte value = *weight++;
		if(value == 0)								//check for divide by 0
			buckets[i] = 0;
		else
			//buckets[i] = (double)prizeWeightTotal_ / (double)value;		//TODO: figure out if this should inverse or not
			buckets[i] = (double)value;

		bucketSum += buckets[i];
	}

	double total=0;
	for(Uint i=0; i < sizeof(PrizeSettings); ++i)
	{
		printf("Chance of prize[%d]: %f%\n", i, buckets[i]/bucketSum*100);
		total += buckets[i]/bucketSum*100;
	}
	printf("Total prize chance: %f%\n", total);
}*/

void SubspacePrizeGenerator::setPrizeTimeToLive(double min, double max)
{
	assert(min <= max);

	timeToLiveMin_ = min;
	timeToLiveMax_ = max;
}

void SubspacePrizeGenerator::setSpawnRange(Uint range, Uint rangeUpgrade)
{
	spawnRange_ = range;
	spawnRangeUpgrade_ = rangeUpgrade;
}

SubspacePrize* SubspacePrizeGenerator::createPrize()
{
	Uint type;

	// Find dimensions of the prize bounding rectangle
	Uint16 distance = spawnRange_ + spawnRangeUpgrade_*population_;
	distance = min(distance, 1024);

	// Fill prize coordinates
	Uint32 xRand, yRand;
	/*if(useSeparatePRNG)
	{
		xRand = rand();
		yRand = rand();
	}
	else
	{*/
		xRand = prng_.getNextG();
		yRand = prng_.getNextG();
	//}
	Uint x = (xRand % (distance - 2)) + ((1024 - distance) >> 1) + 1;
	Uint y = (yRand % (distance - 2)) + ((1024 - distance) >> 1) + 1;
	//Uint x = (prng_.getNextG() % distance) + 1024/2;
	//Uint x = (prng_.getNextG() % distance) + 1024/2;

	// Fill prize type
	type = 0;

	Uint32 prizeValRand;
	/*if(useSeparatePRNG)
		prizeValRand = rand();
	else*/
		prizeValRand = prng_.getNextG();

	assert(prizeWeightTotal_ > 0);

	//random number
	//double randNum = fmod(prizeValRand, bucketSum);

	//find matching bucket - which prize
	/*double currentValue = 0;
	for(Uint i=0; i < sizeof(PrizeSettings); ++i)	//assumes correspondance between PrizeSettings number and Prize enum
	{
		if(randNum >= currentValue && randNum < currentValue+buckets[i])
		{
			type = i;
			break;
		}

		currentValue += buckets[i];
	}*/

	type = getPrize();

	//TODO: implement negative prizes

	// Determine if prize type is negative or not
	/*Uint32 prizeNegRand;
	if(useSeparatePRNG)
		prizeNegRand = rand();
	else
		prizeNegRand = prng_.getNextG();

	if (prizeNegRand % prizeNegativeFactor_ == 0)
		type = -type;*/

	// Fill alive time
	double time = timeToLiveMax_ - timeToLiveMin_;
	
	Uint16 factor;
	/*if(useSeparatePRNG)
		factor = rand();
	else*/
		factor = prng_.getNextG();

	if(time)
		time = fmod(factor, time);		//TODO: make sure this conversion is correct - what the hell is this for?

	time = timeToLiveMin_ + time;
			
	Vector pos;
	tileToObjectCoords(x, y, &pos);

	SubspacePrize* p = new SubspacePrize(pos.x_, pos.y_, (PrizeType)type);
	p->setTimeToLive(time);

	return p;
}

PrizeType SubspacePrizeGenerator::getPrize()
{
	Uint current = 0;
	Uint stop = prng_.getNextG() % prizeWeightTotal_;
	Uint i;

	Byte *pw = (Byte*)&prizeWeights_;
	for(i = 1; i < sizeof(PrizeSettings); ++i)
	{
		if(*pw != 0)
		{
			if(stop >= current && stop < current + (*pw))
				break;
		}

		current += *pw++;
	}
	return (PrizeType)i;
}

PrizeType SubspacePrizeGenerator::getMultiPrize()
{
	Uint current = 0;
	
	if(multiPrizeWeightTotal_ == 0)		//for divide by zero error
		multiPrizeWeightTotal_ = 1;

	Uint stop = rand() % multiPrizeWeightTotal_;
	//Uint stop = 900 ;
	Uint i;

	Byte *pw = (Byte*)&multiPrizeWeights_;
	for(i = 1; i < sizeof(PrizeSettings); ++i)
	{
		if(*pw != 0)
		{
			//if(current >= stop && stop < current + (*pw))
			if(stop <= current + (*pw))
				break;
		}

		current += *pw++;
	}
	return (PrizeType)i;
}
