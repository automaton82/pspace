//David Millman

#ifndef _SUBSPACEDOORSYSTEM_H_
#define _SUBSPACEDOORSYSTEM_H_

#include "Updatable.h"
#include "SubspacePRNG.h"

class SubspaceDoorSystem : 
	public Updatable
{
public:
	SubspaceDoorSystem();
	~SubspaceDoorSystem();

	void setDoorSeed(Uint32 seed);
	void setDoorMode(int mode);
	void setTimestamp(Uint32 timestamp);

	//update animations
	void update(double time);
		
public:

	SS_HEAVY_PRNG prng_;
	Uint32 doorSeed_;
	
	Uint32 timestamp_;

	int mode_;
};

#endif