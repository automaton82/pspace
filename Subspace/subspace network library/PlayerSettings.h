#ifndef _PLAYERSETTINGS_H_
#define _PLAYERSETTINGS_H_

#include <string>
using std::string;

#include "DataTypes.h"

struct PlayerData
{
	PlayerData() :
		shipType(0), acceptsAudio(0),
		flagPoints(0), killPoints(0),
		playerID(0), 
		team(0), wins(0),
		losses(0), turreteeID(0),
		flagsCarried(0),
		hasKoTH(0)
	{
	}

	Byte shipType;
	Byte acceptsAudio;
	string name;
	string squadName;
	Uint32 killPoints;
	Uint32 flagPoints;
	Uint16 playerID;
	Uint16 team;
	Uint16 wins;
	Uint16 losses;
	Uint16 turreteeID;
	Uint16 flagsCarried;
	Byte hasKoTH;
};

union PlayerStates
{
	PlayerStates() :
		u(0)
	{
	}

	struct
	{
		unsigned stealth		: 1;
		unsigned cloaked		: 1;	
		unsigned xradar			: 1;
		unsigned awarp			: 1;
		unsigned flash			: 1;	// Uncloaking, portaling, etc.
		unsigned safety			: 1;	// In a safety zone
		unsigned ufo			: 1;	// *ufo - Illegal usage caught in sg9+
		unsigned pack			: 1;	// ?
	};

	Byte u;
};

union PlayerItems
{
	PlayerItems() :
		u(0)
	{
	}

	struct
	{
		unsigned shields	: 1;	// Has it or not
		unsigned supers		: 1;
		unsigned burst		: 4;	// Item counts
		unsigned repel		: 4;
		unsigned thor		: 4;
		unsigned brick		: 4;
		unsigned decoy		: 4;
		unsigned rocket		: 4;
		unsigned portal		: 4;
		unsigned pack		: 2;	// Probably used somehow
	};

	Uint32 u;
};


enum ShipType
{
	SHIP_Warbird = 0,		// 0
	SHIP_Javelin,
	SHIP_Spider,
	SHIP_Leviathan,
	SHIP_Terrier,
	SHIP_Weasel,
	SHIP_Lancaster,
	SHIP_Shark,
	SHIP_Spectator,		// 8

	NUM_SHIPS
};

ShipType getShipType(int n);
ShipType getShipType(const string& s);	//actually implement this, "warbird" = SHIP_Warbird


#endif