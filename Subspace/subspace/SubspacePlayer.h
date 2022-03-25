#ifndef _SUBSPACEPLAYER_H_
#define _SUBSPACEPLAYER_H_


#include <list>
using std::list;

#include "Rect.h"

#include "ArenaSettings.h"
#include "PlayerSettings.h"
#include "WeaponSettings.h"

#include "PlayerData.h"
#include "SubspaceBanner.h"
#include "SubspaceDefines.h"
#include "SubspaceGameObject.h"
#include "SubspacePrizeGenerator.h"

class SubspaceBall;
class SubspaceBomb;
class SubspaceBrick;
class SubspaceBullet;
class SubspaceBurstBullet;
class SubspaceDecoy;
class SubspaceFlag;
class SubspaceMultiBullet;
class SubspacePortal;
class SubspaceRepel;
class SubspaceThor;
class SubspaceZone;

class SubspacePlayer : 
	public SubspaceGameObject
{
public:

public:
	SubspacePlayer();
	SubspacePlayer(const PlayerData& data);
	~SubspacePlayer();

	void init();

	//Data management
	void setArenaSettings(ArenaSettings* arenaSettings);
	//void setMap(SubspaceMap* map);		//gets map from zone, best interaction?
	void setName(const string& name);
	void setZone(SubspaceZone* zone);

	void setPlayerData(const PlayerData& data, bool allData = false);
	void setScore(int killPoints, int flagPoints, int wins, int losses);	
	int getId() const;

	void updateFromData(const PlayerData& data);

	WeaponData getBulletSettings() const;
	WeaponData getBombSettings() const;
	PlayerItems getItemSettings() const;
	PlayerStates getStateSettings() const;

	const ShipItems& getItems() const;
	const ShipStatus& getStatus() const;
	const ShipWeapons& getWeapons() const;

	const SubspaceBanner* getBanner() const;
	void setBanner(const SubspaceBanner& banner);

	// Ship status data
	Uint getBounty() const;
	double getEnergy() const;
	double getMaxEnergy() const;
	double getMaxPossibleEnergy() const;
	double getRechargePercent() const;
	double getRotationPercent() const;
	ShipType getShipType() const;
	double getSpeedPercent() const;
	Uint getTeam() const;
	double getThrustPercent() const;

	// Player data
	double getAveKill() const;
	Uint getBalls() const;
	Uint getFlags() const;
	double getFlagTime() const;
	Uint getLosses() const;
	string getName() const;
	Uint getPoints() const;
	Uint getRating() const;		//TODO: get actual rating formula
	string getSquad() const;
	Uint getWins() const;
	
	bool isDead() const;
	bool isMyPlayer() const;		//is local player?
	bool isSpectator() const;
	bool isTurret() const;

	//Ship management
	//Uint getSize() const;
	void initShip();
	void initItems();
	void initStatus();
	void initWeapons();

	void changeShip(ShipType ship);
	void changeTeam(int team, ShipType ship);
	void die(bool dead = true);
	void killedPlayer(Uint playerID, Uint bounty);

	//void givePrize(bool allowMultiprize = true);					//random prize
	void givePrize(PrizeType prize);
	void giveMultiprize(Uint numPrizes = 1);
    
	void rechargeEnergy(double time);
	void applyDamage(double damage);

	//Flags
	void giveFlag(SubspaceFlag* flag);
	bool dropFlag(SubspaceFlag* flag);
	void dropFlags();

	//Balls
	void giveBall(SubspaceBall* ball);
	SubspaceBall* dropBall();

	//King of the hill
	bool hasKoth() const;
	void setKothTime(double millseconds);
	double getKothTime() const;

	//Ship upgrades
	void upgradeAntiwarp();
	void upgradeBombs();
	void upgradeBounce();
	void upgradeBricks(Uint num = 1);
	void upgradeBursts(Uint num = 1);
	void upgradeCloak();
	void upgradeDecoys(Uint num = 1);
	void upgradeFullCharge();
	void upgradeGuns();
	void upgradeMaxEnergy();
	void upgradeMultifire();
	void upgradePortals(Uint num = 1);
	void upgradeProximity();
	void upgradeRechargeRate();
	void upgradeRepels(Uint num = 1);
	void upgradeRockets(Uint num = 1);
	void upgradeRotationRate();
	void upgradeShields();
	void upgradeShrapnel();
	void upgradeStealth();
	void upgradeSuper();
	void upgradeThors(Uint num = 1);
	void upgradeThruster();
	void upgradeTopSpeed();
	void upgradeXRadar();

	//Weapon management
												
	//TODO: need separate time delay for firing items?
	void fireWeapon(const WeaponData& weapon);		//TODO: try to phase this out?
	SubspaceBullet* fireBullet();
	SubspaceBomb* fireBomb();
	SubspaceBurstBullet* fireBurst();
	SubspaceDecoy* fireDecoy();
	SubspaceBomb* fireMine();
	SubspaceMultiBullet* fireMultiBullet();
	SubspaceRepel* fireRepel();
	SubspaceThor* fireThor();
	SubspacePortal* firePortal();

	void mineFired();		//inc mine counter
	void mineDestroyed();	//dec mine counter

	bool useBrick();
	bool hasMoreMines() const;
	bool isPortalActive() const;
	bool usePortal();
	bool useRocket();

	void clearPortal();
	double getPortalTime() const;

	double getShieldsTime() const;
	double getShieldsPercent() const;	//100% = 1.0
	double getSuperTime() const;

	//Position management
	void applyThrust(double power, bool reverse = false);
	void thrust(bool maxThrust = false);
	void thrustReverse(bool maxThrust = false);
	void turnLeft(bool maxTurn = false);
	void turnRight(bool maxTurn = false);
	
	void move(const Vector& dir);	//for spectator

	void warp(bool activated = false);
	void warp(int centerX, int centerY, bool passive);

	//State management
	void toggleAntiwarp();
	void toggleCloak();
	void toggleMultifire();
	void toggleStealth();
	void toggleXRadar();

	double getSafeTime() const;
	void setSafe(bool isSafe);
	bool isSafe() const;

	//Turrets
	SubspacePlayer* getTurretParent() const;
	const SubspacePlayerList& getTurrets() const;

	void addTurret(SubspacePlayer* p);
	void clearTurrets();
	void removeTurret(SubspacePlayer* p);
	void setTurretParent(SubspacePlayer* p);
	

	//Physics
	//TODO: move this into a physics library
	Rect getBoundingBox() const;

	////////////////////////////////////

	void update(double timeMultiplier);
	void draw() const;
	void display() const;

private:

	//Special Effects
	void createExhaust(bool reverse, Uint num, double angle, bool rocket); //angle in radians
	void createFlash();

	void setTexture();	//change between ship and turret

private:
	
	SubspaceZone* zone_;


	ArenaSettings* arenaSettings_;
	ShipSettings* shipSettings_;	//for convenience

	//TODO: make separate state management stuff
	//Player settings - states
	PlayerData data_;		//TODO: phase this out
	SubspaceBanner banner_;
	
	//weapons
	ShipWeapons weapons_;	//weapons available
	double bulletFireDelay_;		//time to wait before firing another weapon
	double bombFireDelay_;		//TODO: what is correct way of handling fire delays?
	Uint mines_;

	//items available
	ShipItems items_;

	SubspacePortal* activePortal_;
	bool isPortalActive_;
	//status
	//TODO: add flag, powerball, anything else status related
	ShipStatus status_;

	//for prize generation
	SubspacePrizeGenerator* prizeGenerator_;

	//flags
	typedef vector<SubspaceFlag*> FlagList;
	FlagList flags_;
	double flagTime_;

	//balls
	typedef vector<SubspaceBall*> BallList;
	BallList balls_;

	//king of the hill
	double kothTime_;
	
	//movement
	bool isThrusting_;
	bool isAfterburner_;

	Vector currentThrust_;
	//double thrustPower_;
	double exhaustDisplayDelay_;
	static const double maxExhaustDisplayDelay;	//milliseconds
	Vector moveAccum_;		// for spectator, tiles/ms

	static const double SPECTATOR_MOVE_RATE;
	static const double SPECTATOR_MOVE_RATE_FAST;

	double rocketTime_;

	bool isTurning_;
	double currentTurn_;
	//double turnRate_;

	bool isDead_;

	double safeTime_;

	//Turrets
	SubspacePlayerList turrets_;
	SubspacePlayer* turretParent_;
};

#endif