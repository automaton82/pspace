#ifndef _PLAYERDATA_H_
#define _PLAYERDATA_H_

#include "PlayerSettings.h"
#include "Vector.h"

struct ShipWeapons
{
	void reset()
	{
		guns.bouncing = false;
		guns.level = 0;
		guns.multifire = false;

		bombs.bounces = 0;
		bombs.emp = false;
		bombs.level = 0;
		bombs.shrapnel = 0;
	}

	struct GunInfo
	{
		GunInfo() : bouncing(false), level(0), multifire(false)
		{}

		bool bouncing;
		Uint level;
		bool multifire;
	} guns;

	struct BombInfo
	{
		BombInfo() : bounces(0), emp(false), level(0), shrapnel(0)
		{}

		Uint bounces;
		bool emp;
		Uint level;
		Uint shrapnel;
	} bombs;
};

struct ShipItems
{
	ShipItems() : antiwarp(false), bricks(0), bursts(0), cloak(false), decoys(0), multifire(false), 
		portals(0), proximity(false), repels(0), rockets(0), shields(0), stealth(false), superPower(0),
		thors(0), xRadar(false)
	{}

	void reset()		//reset valid items
	{
		antiwarp = false;
		bricks = 0;
		bursts = 0;
		cloak = false;
		decoys = 0;
		multifire = false;
		portals = 0;
		proximity = false;
		repels = 0;
		rockets = 0;
		shields = 0;
		stealth = false;
		superPower = 0;
		thors = 0;
		xRadar = false;
	}

	bool antiwarp;
	//int bombLevel;	//necessary?
	Uint bricks;
	Uint bursts;
	bool cloak;
	Uint decoys;
	bool multifire;
	Uint portals;
	bool proximity;
	Uint repels;
	Uint rockets;
	int shields;		//timeLeft on shields
	bool stealth;
	int superPower;		//timeLeft on super
	Uint thors;
	bool xRadar;
};

struct ShipStatus
{
	ShipStatus() : bounty(0), flags(0), balls(0), antiwarp(false), cloak(false), stealth(false), 
		xRadar(false), flashing(false), safe(false), energy(0), maxEnergy(0), 
		rechargeRate(0), team(0), ship(SHIP_Spectator), thrustPower(0), turnRate(0), topSpeed(0)
	{}

	void reset()		//reset valid items
	{
		bounty = 0;
		flags = 0;
		balls = 0;
		flashing = false;
		energy = 0;
		maxEnergy = 0;
		rechargeRate = 0;

		thrustPower = 0;
		turnRate = 0;
		topSpeed = 0;
	}

	Uint bounty;
	Uint flags;				//has flag power or not?
	Uint balls;				//holding a powerball

	bool antiwarp;				//are these enabled?
	bool cloak;	
	bool stealth;
	bool xRadar;

	bool flashing;
	bool safe;

	double energy;
	double maxEnergy;
	double rechargeRate;

	Uint team;
	ShipType ship;

	double thrustPower;
	double turnRate;
	double topSpeed;
};
	

#endif