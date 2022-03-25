#ifndef _ARENASETTINGS_H_
#define _ARENASETTINGS_H_

#include "DataTypes.h"

#include "ShipSettings.h"
#include "PrizeSettings.h"


// Arena settings

struct ArenaSettings				// 1428 bytes wide
{	// Initial template by Snrrrub
	unsigned Version : 8;			// 0000 VIE:15
	unsigned ExactDamage : 1;		// 0001 Bullet:ExactDamage:0:1:If damage is to be random or not (1=exact, 0=random) [Continuum .36]
	unsigned HideFlags : 1;			// 0001 Spectator:HideFlags:0:1:If flags are to be shown to specs when they are dropped (1=can't see them) [Continuum .36]
	unsigned NoXRadar : 1;			// 0001 Spectator:NoXRadar:0:1:If specs are allowed to have X (0=yes, 1=no) [Continuum .36]
	unsigned pack0 : 21;			// 0001 (14)

	ShipSettings ships[8];			// 0004 See shipSettings declaration...

	Uint32 BulletDamageLevel;		// 1156 [BulletDamageLevel * 1000] Bullet:BulletDamageLevel:::Maximum amount of damage that a L1 bullet will cause. Formula; damage = squareroot(rand# * (max damage^2 + 1))
	Uint32 BombDamageLevel;			// 1160 [BombDamageLevel * 1000] Bomb:BombDamageLevel:::Amount of damage a bomb causes at its center point (for all bomb levels)
	Uint32 BulletAliveTime;			// 1164 Bullet:BulletAliveTime:::How long bullets live before disappearing (in hundredths of a second)
	Uint32 BombAliveTime;			// 1168 Bomb:BombAliveTime:::Time bomb is alive (in hundredths of a second)
	Uint32 DecoyAliveTime;			// 1172 Misc:DecoyAliveTime:::Time a decoy is alive (in hundredths of a second)
	Uint32 SafetyLimit;				// 1176 Misc:SafetyLimit:::Amount of time that can be spent in the safe zone. (90000 = 15 mins)
	Uint32 FrequencyShift;			// 1180 Misc:FrequencyShift:0:10000:Amount of random frequency shift applied to sounds in the game.
	Uint32 MaxFrequency;			// 1184 Team:MaxFrequency:::Maximum number of frequencies allowed in arena (5 would allow frequencies 0,1,2,3,4)
	Uint32 RepelSpeed;				// 1188 Repel:RepelSpeed:::Speed at which players are repelled
	Uint32 MineAliveTime;			// 1192 Mine:MineAliveTime:0:60000:Time that mines are active (in hundredths of a second)
	Uint32 BurstDamageLevel;		// 1196 [BurstDamageLevel * 1000] Burst:BurstDamageLevel:::Maximum amount of damage caused by a single burst bullet.
	Uint32 BulletDamageUpgrade;		// 1200 [BulletDamageUpgrade * 1000] Bullet:BulletDamageUpgrade:::Amount of extra damage each bullet level will cause
	Uint32 FlagDropDelay;			// 1204 Flag:FlagDropDelay:::Time before flag is dropped by carrier (0=never)
	Uint32 EnterGameFlaggingDelay;	// 1208 Flag:EnterGameFlaggingDelay:::Time a new player must wait before they are allowed to see flags
	Uint32 RocketThrust;			// 1212 Rocket:RocketThrust:::Thrust value given while a rocket is active.
	Uint32 RocketSpeed;				// 1216 Rocket:RocketSpeed:::Speed value given while a rocket is active.
	Uint32 InactiveShrapnelDamage;	// 1220 [InactiveShrapnelDamage * 1000] Shrapnel:InactiveShrapDamage:::Amount of damage shrapnel causes in it's first 1/4 second of life.
	Uint32 WormholeSwitchTime;		// 1224 Wormhole:SwitchTime:::How often the wormhole switches its destination.
	Uint32 ActivateAppShutdownTime;	// 1228 Misc:ActivateAppShutdownTime:::Amount of time a ship is shutdown after application is reactivated (ie. when they come back from windows mode)
	Uint32 ShrapnelSpeed;			// 1232 Shrapnel:ShrapnelSpeed:::Speed that shrapnel travels

	BYTE UNKNOWN0[16];				// 1236 ?

	Uint16 SendRoutePercent;		// 1252 Latency:SendRoutePercent:300:800:Percentage of the ping time that is spent on the ClientToServer portion of the ping. (used in more accurately syncronizing clocks)
	Uint16 BombExplodeDelay;		// 1254 Bomb:BombExplodeDelay:::How long after the proximity sensor is triggered before bomb explodes. (note: it explodes immediately if ship moves away from it after triggering it)
	Uint16 SendPositionDelay;		// 1256 Misc:SendPositionDelay:0:20:Amount of time between position packets sent by client.
	Uint16 BombExplodePixels;		// 1258 Bomb:BombExplodePixels:::Blast radius in pixels for an L1 bomb (L2 bombs double this, L3 bombs triple this)
	Uint16 DeathPrizeTime;			// 1260 Prize:DeathPrizeTime:::How long the prize exists that appears after killing somebody.
	Uint16 JitterTime;				// 1262 Bomb:JitterTime:::How long the screen jitters from a bomb hit. (in hundredths of a second)
	Uint16 EnterDelay;				// 1264 Kill:EnterDelay:::How long after a player dies before he can re-enter the game.
	Uint16 EngineShutdownTime;		// 1266 Prize:EngineShutdownTime:::Time the player is affected by an 'Engine Shutdown' Prize (in hundredth of a second)
	Uint16 ProximityDistance;		// 1268 Bomb:ProximityDistance:::Radius of proximity trigger in tiles.  Each bomb level adds 1 to this amount.
	Uint16 BountyIncreaseForKill;	// 1270 Kill:BountyIncreaseForKill:::Number of points added to players bounty each time he kills an opponent.
	Uint16 BounceFactor;			// 1272 Misc:BounceFactor:::How bouncy the walls are (16=no-speed-loss)
	Uint16 MapZoomFactor;			// 1274 Radar:MapZoomFactor:8:1000:A number representing how far you can see on radar.
	Uint16 MaxBonus;				// 1276 Kill:MaxBonus:::Let's ignore these for now. Or let's not. :) This is if you have flags, can add more points per a kill. Founded by MGB
	Uint16 MaxPenalty;				// 1278 Kill:MaxPenalty:::Let's ignore these for now. Or let's not. :) This is if you have flags, can take away points per a kill. Founded by MGB
	Uint16 RewardBase;				// 1280 Kill:RewardBase:::Let's ignore these for now. Or let's not. :) This is shown added to a person's bty, but isn't added from points for a kill. Founded by MGB
	Uint16 RepelTime;				// 1282 Repel:RepelTime:::Time players are affected by the repel (in hundredths of a second)
	Uint16 RepelDistance;			// 1284 Repel:RepelDistance:::Number of pixels from the player that are affected by a repel.
	Uint16 HelpTickerDelay;			// 1286 Misc:TickerDelay:::Amount of time between ticker help messages.
	Uint16 FlaggerOnRadar;			// 1288 Flag:FlaggerOnRadar:::Whether the flaggers appear on radar in red 0=no 1=yes
	Uint16 FlaggerKillMultiplier;	// 1290 Flag:FlaggerKillMultiplier:::Number of times more points are given to a flagger (1 = double points, 2 = triple points)
	Uint16 PrizeFactor;				// 1292 Prize:PrizeFactor:::Number of prizes hidden is based on number of players in game.  This number adjusts the formula, higher numbers mean more prizes. (*Note: 10000 is max, 10 greens per person)
	Uint16 PrizeDelay;				// 1294 Prize:PrizeDelay:::How often prizes are regenerated (in hundredths of a second)
	Uint16 PrizeMinimumVirtual;		// 1296 Prize:MinimumVirtual:::Distance from center of arena that prizes/flags/soccer-balls will generate
	Uint16 PrizeUpgradeVirtual;		// 1298 Prize:UpgradeVirtual:::Amount of additional distance added to MinimumVirtual for each player that is in the game.
	Uint16 PrizeMaxExist;			// 1300 Prize:PrizeMaxExist:::Maximum amount of time that a hidden prize will remain on screen. (actual time is random)
	Uint16 PrizeMinExist;			// 1302 Prize:PrizeMinExist:::Minimum amount of time that a hidden prize will remain on screen. (actual time is random)
	Uint16 PrizeNegativeFactor;		// 1304 Prize:PrizeNegativeFactor:::Odds of getting a negative prize.  (1 = every prize, 32000 = extremely rare)
	Uint16 DoorDelay;				// 1306 Door:DoorDelay:::How often doors attempt to switch their state.
	Uint16 AntiwarpPixels;			// 1308 Toggle:AntiWarpPixels:::Distance Anti-Warp affects other players (in pixels) (note: enemy must also be on radar)
	Sint16 DoorMode;				// 1310 Door:DoorMode:::Door mode (-2=all doors completely random, -1=weighted random (some doors open more often than others), 0-255=fixed doors (1 bit of byte for each door specifying whether it is open or not)
	Uint16 FlagBlankDelay;			// 1312 Flag:FlagBlankDelay:::Amount of time that a user can get no data from server before flags are hidden from view for 10 seconds.
	Uint16 NoDataFlagDropDelay;		// 1314 Flag:NoDataFlagDropDelay:::Amount of time that a user can get no data from server before flags he is carrying are dropped.
	Uint16 MultiPrizeCount;			// 1316 Prize:MultiPrizeCount:::Number of random 'Greens' given with a 'MultiPrize'
	Uint16 BrickTime;				// 1318 Brick:BrickTime:::How long bricks last (in hundredths of a second)
	Uint16 WarpRadiusLimit;			// 1320 Misc:WarpRadiusLimit:::When ships are randomly placed in the arena, this parameter will limit how far from the center of the arena they can be placed (1024=anywhere)
	Uint16 EBombShutdownTime;		// 1322 Bomb:EBombShutdownTime:::Maximum time recharge is stopped on players hit with an EMP bomb.
	Uint16 EBombDamagePercent;		// 1324 Bomb:EBombDamagePercent:::Percentage of normal damage applied to an EMP bomb 0=0% 1000=100% 2000=200%
	Uint16 RadarNeutralSize;		// 1326 Radar:RadarNeutralSize:0:1024:Size of area between blinded radar zones (in pixels)
	Uint16 WarpPointDelay;			// 1328 Misc:WarpPointDelay:::How long a Portal point is active.
	Uint16 NearDeathLevel;			// 1330 Misc:NearDeathLevel:::Amount of energy that constitutes a near-death experience (ships bounty will be decreased by 1 when this occurs -- used for dueling zone)
	Uint16 BBombDamagePercent;		// 1332 Bomb:BBombDamagePercent:::Percentage of normal damage applied to a bouncing bomb 0=0% 1000=100% 2000=200%
	Uint16 ShrapnelDamagePercent;	// 1334 Shrapnel:ShrapnelDamagePercent:::Percentage of normal damage applied to shrapnel (relative to bullets of same level) 0=0% 1000=100% 2000=200%
	Uint16 ClientSlowPacketTime;	// 1336 Latency:ClientSlowPacketTime:20:200:Amount of latency S2C that constitutes a slow packet.
	Uint16 FlagDropResetReward;		// 1338 Flag:FlagDropResetReward:::Minimum kill reward that a player must get in order to have his flag drop timer reset.
	Uint16 FlaggerFireCostPercent;	// 1340 Flag:FlaggerFireCostPercent:::Percentage of normal weapon firing cost for flaggers 0=Super 1000=100% 2000=200%
	Uint16 FlaggerDamagePercent;	// 1342 Flag:FlaggerDamagePercent:::Percentage of normal damage received by flaggers 0=Invincible 1000=100% 2000=200%
	Uint16 FlaggerBombFireDelay;	// 1344 Flag:FlaggerBombFireDelay:::Delay given to flaggers for firing bombs (0=ships normal firing rate -- note: please do not set this number less than 20)
	Uint16 SoccerPassDelay;			// 1346 Soccer:PassDelay:0:10000:How long after the ball is fired before anybody can pick it up (in hundredths of a second)
	Uint16 SoccerBallBlankDelay;	// 1348 Soccer:BallBlankDelay:::Amount of time a player can receive no data from server and still pick up the soccer ball.
	Uint16 S2CNoDataKickoutDelay;	// 1350 Latency:S2CNoDataKickoutDelay:100:32000:Amount of time a user can receive no data from server before connection is terminated.
	Uint16 FlaggerThrustAdjustment;	// 1352 Flag:FlaggerThrustAdjustment:::Amount of thrust adjustment player carrying flag gets (negative numbers mean less thrust)
	Uint16 FlaggerSpeedAdjustment;	// 1354 Flag:FlaggerSpeedAdjustment:::Amount of speed adjustment player carrying flag gets (negative numbers mean slower)
	Uint16 CliSlowPacketSampleSize;	// 1356 Latency:ClientSlowPacketSampleSize:50:1000:Number of packets to sample S2C before checking for kickout.

	BYTE UNKNOWN1[10];				// 1358 ?

	BYTE RandomShrapnel;			// 1368 Shrapnel:Random:0:1:Whether shrapnel spreads in circular or random patterns 0=circular 1=random
	BYTE SoccerBallBounce;			// 1369 Soccer:BallBounce:0:1:Whether the ball bounces off walls (0=ball go through walls, 1=ball bounces off walls)
	BYTE SoccerAllowBombs;			// 1370 Soccer:AllowBombs:0:1:Whether the ball carrier can fire his bombs (0=no 1=yes)
	BYTE SoccerAllowGuns;			// 1371 Soccer:AllowGuns:0:1:Whether the ball carrier can fire his guns (0=no 1=yes)
	BYTE SoccerMode;				// 1372 Soccer:Mode:0:6:Goal configuration (0=any goal, 1=left-half/right-half, 2=top-half/bottom-half, 3=quadrants-defend-one-goal, 4=quadrants-defend-three-goals, 5=sides-defend-one-goal, 6=sides-defend-three-goals)
	BYTE MaxPerTeam;				// 1373 Team:MaxPerTeam:::Maximum number of players on a non-private frequency
	BYTE MaxPerPrivateTeam;			// 1374 Team:MaxPerPrivateTeam:::Maximum number of players on a private frequency (0=same as MaxPerTeam)
	BYTE TeamMaxMines;				// 1375 Mine:TeamMaxMines:0:32000:Maximum number of mines allowed to be placed by an entire team
	BYTE WormholeGravityBombs;		// 1376 Wormhole:GravityBombs:0:1:Whether a wormhole affects bombs (0=no 1=yes)
	BYTE BombSafety;				// 1377 Bomb:BombSafety:0:1:Whether proximity bombs have a firing safety (0=no 1=yes).  If enemy ship is within proximity radius, will it allow you to fire.
	BYTE MessageReliable;			// 1378 Message:MessageReliable:0:1:Whether messages are sent reliably.
	BYTE TakePrizeReliable;			// 1379 Prize:TakePrizeReliable:0:1:Whether prize packets are sent reliably (C2S)
	BYTE AllowAudioMessages;		// 1380 Message:AllowAudioMessages:0:1:Whether players can send audio messages (0=no 1=yes)
	BYTE PrizeHideCount;			// 1381 Prize:PrizeHideCount:::Number of prizes that are regenerated every PrizeDelay.
	BYTE ExtraPositionData;			// 1382 Misc:ExtraPositionData:0:1:Whether regular players receive sysop data about a ship (leave this at zero)
	BYTE SlowFrameCheck;			// 1383 Misc:SlowFrameCheck:0:1:Whether to check for slow frames on the client (possible cheat technique) (flawed on some machines, do not use)
	BYTE CarryFlags;				// 1384 Flag:CarryFlags:0:2:Whether the flags can be picked up and carried (0=no, 1=yes, 2=yes-one at a time)
	BYTE AllowSavedShip;			// 1385 Misc:AllowSavedShips:0:1:Whether saved ships are allowed (do not allow saved ship in zones where sub-arenas may have differing parameters) 1 = Savedfrom last arena/lagout, 0 = New Ship when entering arena/zone
	BYTE RadarMode;					// 1386 Radar:RadarMode:0:4:Radar mode (0=normal, 1=half/half, 2=quarters, 3=half/half-see team mates, 4=quarters-see team mates)
	BYTE VictoryMusic;				// 1387 Misc:VictoryMusic:0:1:Whether the zone plays victory music or not.
	BYTE FlaggerGunUpgrade;			// 1388 Flag:FlaggerGunUpgrade:0:1:Whether the flaggers get a gun upgrade 0=no 1=yes
	BYTE FlaggerBombUpgrade;		// 1389 Flag:FlaggerBombUpgrade:0:1:Whether the flaggers get a bomb upgrade 0=no 1=yes
	BYTE SoccerUseFlagger;			// 1390 Soccer:UseFlagger:0:1:If player with soccer ball should use the Flag:Flagger* ship adjustments or not (0=no, 1=yes)
	BYTE SoccerBallLocation;		// 1391 Soccer:BallLocation:0:1:Whether the balls location is displayed at all times or not (0=not, 1=yes)

	BYTE UNKNOWN2[8];				// 1392 ?

	PrizeSettings prizeSettings;	// 1400 See prizeSettings declaration...
};

#endif