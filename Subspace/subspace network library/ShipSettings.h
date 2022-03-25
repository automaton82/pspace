//TODO: who wrote this file?

#ifndef _SHIPSETTINGS_H_
#define _SHIPSETTINGS_H_

#include "DataTypes.h"

// Ship settings

struct ShipSettings					// 144 bytes wide, offsets are for warbird
{	// Mostly Snrrrub
	Uint16 SuperTime;				// 0004 All:SuperTime:1::How long Super lasts on the ship (in hundredths of a second)
	Uint16 UNKNOWN0;				// 0006 (100)	Salt for actual super time?
	Uint16 ShieldsTime;				// 0008 All:ShieldsTime:1::How long Shields lasts on the ship (in hundredths of a second)
	Uint16 UNKNOWN1;				// 0010 (30)	Salt for actual shields time?
	Uint16 Gravity;					// 0012 All:Gravity:::Uses this formula, where R = raduis (tiles) and g = this setting; R = 1.325 * (g ^ 0.507)  IE: If set to 500, then your ship will start to get pulled in by the wormhole once you come within 31 tiles of it
	Uint16 GravityTopSpeed;			// 0014 All:GravityTopSpeed:::Ship are allowed to move faster than their maximum speed while effected by a wormhole.  This determines how much faster they can go (0 = no extra speed)
	Uint16 BulletFireEnergy;		// 0016 All:BulletFireEnergy:::Amount of energy it takes a ship to fire a single L1 bullet
	Uint16 MultiFireEnergy;			// 0018 All:MultiFireEnergy:::Amount of energy it takes a ship to fire multifire L1 bullets
	Uint16 BombFireEnergy;			// 0020 All:BombFireEnergy:::Amount of energy it takes a ship to fire a single bomb
	Uint16 BombFireEnergyUpgrade;	// 0022 All:BombFireEnergyUpgrade:::Extra amount of energy it takes a ship to fire an upgraded bomb. ie. L2 = BombFireEnergy+BombFireEnergyUpgrade
	Uint16 MineFireEnergy;			// 0024 All:LandmineFireEnergy:::Amount of energy it takes a ship to place a single L1 mine
	Uint16 MineFireEnergyUpgrade;	// 0026 All:LandmineFireEnergyUpgrade:::Extra amount of energy it takes to place an upgraded landmine.  ie. L2 = LandmineFireEnergy+LandmineFireEnergyUpgrade
	Uint16 BulletSpeed;				// 0028 All:BulletSpeed:::How fast bullets travel
	Uint16 BombSpeed;				// 0030 All:BombSpeed:::How fast bombs travel

	unsigned SeeBombLevel : 2;		// 0032 All:SeeBombLevel:0:4:If ship can see bombs on radar (0=Disabled, 1=All, 2=L2 and up, 3=L3 and up, 4=L4 bombs only) [Continuum .36]
	unsigned DisableFastBombs : 1;	// 0032 All:DisableFastShooting:0:1:If firing bullets, bombs, or thors is disabled after using afterburners (1=enabled) [Continuum .36]
	unsigned pack0 : 5;				// 0032 Unused
	unsigned Radius : 8;			// 0033 All:Radius:::The ship's radius from center to outside, in pixels. Standard value is 14 pixels. [Continuum .37]
	unsigned MultiFireAngle : 16;	// 0034 All:MultiFireAngle:::Angle spread between multi-fire bullets and standard forward firing bullets. (111 = 1 degree, 1000 = 1 ship-rotation-point)

	Uint16 CloakEnergy;				// 0036 All:CloakEnergy:0:32000:Amount of energy required to have 'Cloak' activated (thousanths per hundredth of a second)
	Uint16 StealthEnergy;			// 0038 All:StealthEnergy:0:32000:Amount of energy required to have 'Stealth' activated (thousanths per hundredth of a second)
	Uint16 AntiWarpEnergy;			// 0040 All:AntiWarpEnergy:0:32000:Amount of energy required to have 'Anti-Warp' activated (thousanths per hundredth of a second)
	Uint16 XRadarEnergy;			// 0042 All:XRadarEnergy:0:32000:Amount of energy required to have 'X-Radar' activated (thousanths per hundredth of a second)
	Uint16 MaximumRotation;			// 0044 All:MaximumRotation:::Maximum rotation rate of the ship (0 = can't rotate, 400 = full rotation in 1 second)
	Uint16 MaximumThrust;			// 0046 All:MaximumThrust:::Maximum thrust of ship (0 = none)
	Uint16 MaximumSpeed;			// 0048 All:MaximumSpeed:::Maximum speed of ship (0 = can't move)
	Uint16 MaximumRecharge;			// 0050 All:MaximumRecharge:::Maximum recharge rate, or how quickly this ship recharges its energy.
	Uint16 MaximumEnergy;			// 0052 All:MaximumEnergy:::Maximum amount of energy that the ship can have.
	Uint16 InitialRotation;			// 0054 All:InitialRotation:::Initial rotation rate of the ship (0 = can't rotate, 400 = full rotation in 1 second)
	Uint16 InitialThrust;			// 0056 All:InitialThrust:::Initial thrust of ship (0 = none)
	Uint16 InitialSpeed;			// 0058 All:InitialSpeed:::Initial speed of ship (0 = can't move)
	Uint16 InitialRecharge;			// 0060 All:InitialRecharge:::Initial recharge rate, or how quickly this ship recharges its energy.
	Uint16 InitialEnergy;			// 0062 All:InitialEnergy:::Initial amount of energy that the ship can have.
	Uint16 UpgradeRotation;			// 0064 All:UpgradeRotation:::Amount added per 'Rotation' Prize
	Uint16 UpgradeThrust;			// 0066 All:UpgradeThrust:::Amount added per 'Thruster' Prize
	Uint16 UpgradeSpeed;			// 0068 All:UpgradeSpeed:::Amount added per 'Speed' Prize
	Uint16 UpgradeRecharge;			// 0070 All:UpgradeRecharge:::Amount added per 'Recharge Rate' Prize
	Uint16 UpgradeEnergy;			// 0072 All:UpgradeEnergy:::Amount added per 'Energy Upgrade' Prize
	Uint16 AfterburnerEnergy;		// 0074 All:AfterburnerEnergy:::Amount of energy required to have 'Afterburners' activated.
	Uint16 BombThrust;				// 0076 All:BombThrust:::Amount of back-thrust you receive when firing a bomb.
	Uint16 BurstSpeed;				// 0078 All:BurstSpeed:::How fast the burst shrapnel is for this ship.
	Uint16 TurretThrustPenalty;		// 0080 All:TurretThrustPenalty:::Amount the ship's thrust is decreased with a turret riding
	Uint16 TurretSpeedPenalty;		// 0082 All:TurretSpeedPenalty:::Amount the ship's speed is decreased with a turret riding
	Uint16 BulletFireDelay;			// 0084 All:BulletFireDelay:::delay that ship waits after a bullet is fired until another weapon may be fired (in hundredths of a second)
	Uint16 MultiFireDelay;			// 0086 All:MultiFireDelay:::delay that ship waits after a multifire bullet is fired until another weapon may be fired (in hundredths of a second)
	Uint16 BombFireDelay;			// 0088 All:BombFireDelay:::delay that ship waits after a bomb is fired until another weapon may be fired (in hundredths of a second)
	Uint16 LandmineFireDelay;		// 0090 All:LandmineFireDelay:::delay that ship waits after a mine is fired until another weapon may be fired (in hundredths of a second)
	Uint16 RocketTime;				// 0092 All:RocketTime:::How long a Rocket lasts (in hundredths of a second)
	Uint16 InitialBounty;			// 0094 All:InitialBounty:::Number of 'Greens' given to ships when they start
	Uint16 DamageFactor;			// 0096 All:DamageFactor:::How likely a the ship is to take damamage (ie. lose a prize) (0=special-case-never, 1=extremely likely, 5000=almost never)
	Uint16 PrizeShareLimit;			// 0098 All:PrizeShareLimit:::Maximum bounty that ships receive Team Prizes
	Uint16 AttachBounty;			// 0100 All:AttachBounty:::Bounty required by ships to attach as a turret
	Uint16 SoccerThrowTime;			// 0102 All:SoccerThrowTime:::Time player has to carry soccer ball (in hundredths of a second)
	Uint16 SoccerBallFriction;		// 0104 All:SoccerBallFriction:::Amount the friction on the soccer ball (how quickly it slows down -- higher numbers mean faster slowdown)
	Uint16 SoccerBallProximity;		// 0106 All:SoccerBallProximity:::How close the player must be in order to pick up ball (in pixels)
	Uint16 SoccerBallSpeed;			// 0108 All:SoccerBallSpeed:::Initial speed given to the ball when fired by the carrier.

	BYTE TurretLimit;				// 0110 All:TurretLimit:::Number of turrets allowed on a ship.
	BYTE BurstShrapnel;				// 0111 All:BurstShrapnel:::Number of bullets released when a 'Burst' is activated
	BYTE MaxMines;					// 0112 All:MaxMines:::Maximum number of mines allowed in ships
	BYTE RepelMax;					// 0113 All:RepelMax:::Maximum number of Repels allowed in ships
	BYTE BurstMax;					// 0114 All:BurstMax:::Maximum number of Bursts allowed in ships
	BYTE DecoyMax;					// 0115 All:DecoyMax:::Maximum number of Decoys allowed in ships
	BYTE ThorMax;					// 0116 All:ThorMax:::Maximum number of Thor's Hammers allowed in ships
	BYTE BrickMax;					// 0117 All:BrickMax:::Maximum number of Bricks allowed in ships
	BYTE RocketMax;					// 0118 All:RocketMax:::Maximum number of Rockets allowed in ships
	BYTE PortalMax;					// 0119 All:PortalMax:::Maximum number of Portals allowed in ships
	BYTE InitialRepel;				// 0120 All:InitialRepel:::Initial number of Repels given to ships when they start
	BYTE InitialBurst;				// 0121 All:InitialBurst:::Initial number of Bursts given to ships when they start
	BYTE InitialBrick;				// 0122 All:InitialBrick:::Initial number of Bricks given to ships when they start
	BYTE InitialRocket;				// 0123 All:InitialRocket:::Initial number of Rockets given to ships when they start
	BYTE InitialThor;				// 0124 All:InitialThor:::Initial number of Thor's Hammers given to ships when they start
	BYTE InitialDecoy;				// 0125 All:InitialDecoy:::Initial number of Decoys given to ships when they start
	BYTE InitialPortal;				// 0126 All:InitialPortal:::Initial number of Portals given to ships when they start
	BYTE BombBounceCount;			// 0127 All:BombBounceCount:::Number of times a ship's bombs bounce before they explode on impact

	unsigned ShrapnelMax	: 5;	// 0128 All:ShrapnelMax:0:31:Maximum amount of shrapnel released from a ship's bomb
	unsigned ShrapnelRate	: 5;	// 0128 All:ShrapnelRate:0:31:Amount of additional shrapnel gained by a 'Shrapnel Upgrade' prize.

	unsigned CloakStatus	: 2;	// 0129 All:CloakStatus:0:2:Whether ships are allowed to receive 'Cloak' 0=no 1=yes 2=yes/start-with
	unsigned StealthStatus	: 2;	// 0129 All:StealthStatus:0:2:Whether ships are allowed to receive 'Stealth' 0=no 1=yes 2=yes/start-with
	unsigned XRadarStatus	: 2;	// 0129 All:XRadarStatus:0:2:Whether ships are allowed to receive 'X-Radar' 0=no 1=yes 2=yes/start-with
	unsigned AntiwarpStatus	: 2;	// 0130 All:AntiWarpStatus:0:2:Whether ships are allowed to receive 'Anti-Warp' 0=no 1=yes 2=yes/start-with

	unsigned InitialGuns	: 2;	// 0130 All:InitialGuns:0:3:Initial level a ship's guns fire 0=no guns
	unsigned MaxGuns		: 2;	// 0130 All:MaxGuns:0:3:Maximum level a ship's guns can fire 0=no guns
	unsigned InitialBombs	: 2;	// 0130 All:InitialBombs:0:3:Initial level a ship's bombs fire 0=no bombs
	unsigned MaxBombs		: 2;	// 0131 All:MaxBombs:0:3:Maximum level a ship's bombs can fire 0=no bombs

	unsigned DoubleBarrel	: 1;	// 0131 All:DoubleBarrel:0:1:Whether ships fire with double barrel bullets 0=no 1=yes
	unsigned EmpBomb		: 1;	// 0131 All:EmpBomb:0:1:Whether ships fire EMP bombs 0=no 1=yes
	unsigned SeeMines		: 1;	// 0131 All:SeeMines:0:1:Whether ships see mines on radar 0=no 1=yes
	unsigned UNKNOWN2		: 3;	// 0131 ?

	BYTE UNKNOWN3[16];				// 0132 ?
};

#endif