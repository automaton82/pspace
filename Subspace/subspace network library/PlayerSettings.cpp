#include "PlayerSettings.h"

ShipType getShipType(int n)
{
	return (ShipType)n;
}

ShipType getShipType(const string& s)
{
	return SHIP_Warbird;
}
