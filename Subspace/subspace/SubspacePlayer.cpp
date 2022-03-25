//David Millman

#include "SubspacePlayer.h"

#include <algorithm>
#include "Rect.h"
#include "MathUtil.h"
using namespace MathUtil;

#include "SubspaceSound.h"
#include "SubspaceGlobal.h"

#include "SubspaceBall.h"
#include "SubspaceBanner.h"
#include "SubspaceBrick.h"
#include "SubspaceBullet.h"
#include "SubspaceBurstBullet.h"
#include "SubspaceDecoy.h"
#include "SubspaceExhaust.h"
#include "SubspaceFlag.h"
#include "SubspaceFlash.h"
#include "SubspaceMultiBullet.h"
#include "SubspacePortal.h"
#include "SubspaceRepel.h"
#include "SubspaceThor.h"
#include "SubspaceUtil.h"
#include "SubspaceZone.h"

const double SubspacePlayer::maxExhaustDisplayDelay = 75.0;	//milliseconds

const double SubspacePlayer::SPECTATOR_MOVE_RATE = 1.0;
const double SubspacePlayer::SPECTATOR_MOVE_RATE_FAST = 2.0;

SubspacePlayer::SubspacePlayer()
{
	init();
}

SubspacePlayer::SubspacePlayer(const PlayerData& data)
{	
	init();
	this->setPlayerData(data, true);
}

SubspacePlayer::~SubspacePlayer()
{
}

void SubspacePlayer::init()
{
	/*TextureAnimated tex;
	tex.load("graphics/ships.bm2", false);
	tex.setFrameSize(36, 36);
	tex.setFrameRange(0, 40);

    this->setTexture(tex);*/

	zone_ = 0;
	
	turretParent_ = 0;
	turrets_.clear();

	ShipWeapons wepTemp;
	weapons_ = wepTemp;
	mines_ = 0;

	bulletFireDelay_ = 0;
	bombFireDelay_ = 0;

	prizeGenerator_ = 0;

	flagTime_ = 0;
	//setSize(32);
	kothTime_ = 0;

	exhaustDisplayDelay_ = 0;
	rocketTime_ = 0;

	activePortal_ = 0;
	isPortalActive_ = false;

	safeTime_ = 0;

	setTexture();
	setSize(getTexture()->getFrameWidth());		//TODO: what's the global setting for the size?
}

/*******************
 * Data Management *
 *******************/

void SubspacePlayer::setArenaSettings(ArenaSettings* arenaSettings)
{
	arenaSettings_ = arenaSettings;
	shipSettings_ = &arenaSettings->ships[status_.ship];
}

void SubspacePlayer::setName(const string& name)
{
	//data_.info.name = name;
}

void SubspacePlayer::setZone(SubspaceZone* zone)
{
	zone_ = zone;

	prizeGenerator_ = &zone_->getPrizeGenerator();
}

void SubspacePlayer::setPlayerData(const PlayerData& d, bool allData)
{
	//assert(0);		// TODO: this may not be correct

    //this might be dangerous
	if(allData)
	{
		data_ = d;

		//d.flagPoints;
		status_.flags = d.flagsCarried;
		//d.hasKoTH;
		//d.killPoints;
		//d.losses;
		//d.name;
		status_.ship = (ShipType)d.shipType;
		//d.squadName;
		status_.team = d.team;
		//d.turreteeID;
		//d.wins;		
	}

	if(allData)
	{
		TextureAnimated* tex = this->getTexture();
		if(tex)
		{
			Uint shipType = status_.ship;
			tex->setFrameRange(40*shipType, 40*shipType + 40);
			//tex->setFrameSize(36, 36);
		}
	}
}

void SubspacePlayer::setScore(int killPoints, int flagPoints, int wins, int losses)
{
	data_.killPoints = killPoints;
	data_.flagPoints = flagPoints;
	data_.wins = wins;
	data_.losses = losses;
}

/*const PlayerData* SubspacePlayer::getPlayerData() const
{	
	return &data_;
}

PlayerData* SubspacePlayer::getPlayerData()
{
	return &data_;
}*/

const SubspaceBanner* SubspacePlayer::getBanner() const
{
	return &banner_;
}

void SubspacePlayer::setBanner(const SubspaceBanner& banner)
{
	banner_ = banner;
}

int SubspacePlayer::getId() const
{
	return data_.playerID;
}

WeaponData SubspacePlayer::getBulletSettings() const
{
	WeaponData w;
	w.u = 0;
	w.bouncing = weapons_.guns.bouncing;
	w.level = weapons_.guns.level-1;
	w.alternate = weapons_.guns.multifire;
	if(w.bouncing)
		w.type = WEP_BouncingBullet;
	else
		w.type = WEP_Bullet;
	
	return w;
}

WeaponData SubspacePlayer::getBombSettings() const
{
	WeaponData w;
	w.u = 0;
	if(weapons_.bombs.bounces > 0)
		w.bouncing = 1;
	w.emp = weapons_.bombs.emp;
	w.shrapCount = weapons_.bombs.shrapnel;
	w.isBomb = 1;
	w.level = weapons_.bombs.level-1;
	if(items_.proximity)
        w.type = WEP_ProximityBomb;
	else
		w.type = WEP_Bomb;

	return w;
}

PlayerItems SubspacePlayer::getItemSettings() const
{
	PlayerItems i;
	i.u = 0;
	i.brick = items_.bricks;
	i.burst = items_.bursts;
	i.decoy = items_.decoys;
	i.portal = items_.portals;
	i.repel = items_.repels;
	i.rocket = items_.rockets;
	i.shields = items_.shields;
	i.supers = items_.superPower;
	i.thor = items_.thors;

	return i;
}

PlayerStates SubspacePlayer::getStateSettings() const
{
	PlayerStates s;
	s.u = 0;
	s.awarp = status_.antiwarp;
	s.cloaked = status_.cloak;
	s.flash = status_.flashing;
	s.safety = 0;					//TODO: implement safety zones
	s.stealth = status_.stealth;
	s.ufo = 0;						//TODO: implement ufo?
	s.xradar = status_.xRadar;
	
	return s;
}

const ShipItems& SubspacePlayer::getItems() const
{
	return items_;	
}

const ShipStatus& SubspacePlayer::getStatus() const
{
	return status_;
}

const ShipWeapons& SubspacePlayer::getWeapons() const
{
	return weapons_;
}

Uint SubspacePlayer::getBounty() const
{
	return status_.bounty;
}

double SubspacePlayer::getEnergy() const
{
	return status_.energy;
}

double SubspacePlayer::getMaxEnergy() const
{
	return status_.maxEnergy;
}

double SubspacePlayer::getMaxPossibleEnergy() const
{
	return shipSettings_->MaximumEnergy;
}

double SubspacePlayer::getRechargePercent() const
{
	return status_.rechargeRate / (double)shipSettings_->MaximumRecharge * 100.0;
}

double SubspacePlayer::getRotationPercent() const
{
	return status_.turnRate / (double)shipSettings_->MaximumRotation * 100.0;
}

ShipType SubspacePlayer::getShipType() const
{
	return status_.ship;
}

double SubspacePlayer::getSpeedPercent() const
{
	return status_.topSpeed / (double)shipSettings_->MaximumSpeed * 100.0;
}

Uint SubspacePlayer::getTeam() const
{
	return status_.team;
}

double SubspacePlayer::getThrustPercent() const
{
	return status_.thrustPower / (double)shipSettings_->MaximumThrust * 100.0;
}


double SubspacePlayer::getAveKill() const
{
	double retval = 0;

	if(data_.wins > 0)
		retval = (double)data_.killPoints / (double)data_.wins;

	return retval;
}

Uint SubspacePlayer::getBalls() const
{
	return status_.balls;
}

Uint SubspacePlayer::getFlags() const
{
	//TODO: phase out all data_.info and data_ stuff
	//return data_.info.flagsCarried;
	return status_.flags;
}

double SubspacePlayer::getFlagTime() const
{
	return flagTime_;
}

Uint SubspacePlayer::getLosses() const
{
	return data_.losses;
}

string SubspacePlayer::getName() const
{
	return data_.name;
}

Uint SubspacePlayer::getPoints() const
{
	return data_.killPoints + data_.flagPoints;
}

Uint SubspacePlayer::getRating() const
{
	return 0;
	//return (Uint)(getWins() / (getLosses()+1) * getAveKill());
}

string SubspacePlayer::getSquad() const
{
	return data_.squadName;
}

Uint SubspacePlayer::getWins() const
{
	return data_.wins;
}


bool SubspacePlayer::isDead() const
{
	return isDead_;
}

bool SubspacePlayer::isMyPlayer() const
{
	if(!zone_)
		return false;
	else
		return (zone_->getMyPlayer() == this);
}

bool SubspacePlayer::isSpectator() const
{
	return (status_.ship == SHIP_Spectator);	//TODO: or team equals spectator team
}

bool SubspacePlayer::isTurret() const
{
	return (turretParent_ != 0);
}

/*******************
 * Ship Management *
 *******************/

/*Uint SubspacePlayer::getSize() const
{
	//return getTexture()->getFrameWidth();
	return size_;
}*/

void SubspacePlayer::initShip()
{
	shipSettings_ = &arenaSettings_->ships[status_.ship];

	if(status_.ship != SHIP_Spectator)
		warp();

	//status_.reset();
	
	//clear items
	initItems();

	//weapons
	initWeapons();

	//clear status
	initStatus();	//gives bounty too

	//movement
	//data_.bounty = shipSettings_->InitialBounty;
    /*for(int i=0; i < shipSettings_->InitialBounty; ++i)
	{
		this->givePrize();
	}*/
	//status_.bounty = shipSettings_->InitialBounty;

	//drop any flags
	//TODO: actually drop, or just not use them anymore?
	//zone_->playerDropFlag(getId(), true);	
	dropFlags();

	velocity_ = Vector(0, 0, 0);

	activePortal_ = 0;
	isPortalActive_ = false;

	rocketTime_ = 0;

	setTexture();
}

void SubspacePlayer::initItems()
{
	items_.reset();

	if(isSpectator())
	{
		items_.xRadar = true;
		return;
	}

	if(!shipSettings_)
		return;

	if(shipSettings_->AntiwarpStatus == 2)
		items_.antiwarp = true;

	upgradeBricks(shipSettings_->InitialBrick);
	upgradeBursts(shipSettings_->InitialBurst);

	if(shipSettings_->CloakStatus == 2)
		items_.cloak = true;

	upgradeDecoys(shipSettings_->InitialDecoy);

	items_.multifire = false;						//TODO: is there an init for this?

	upgradePortals(shipSettings_->InitialPortal);
	items_.proximity = false;						//TODO: is there an init for this?
	upgradeRepels(shipSettings_->InitialRepel);
	upgradeRockets(shipSettings_->InitialRocket);

	if(shipSettings_->StealthStatus == 2)
		items_.stealth = true;

	upgradeThors(shipSettings_->InitialThor);

	if(shipSettings_->XRadarStatus == 2)
		items_.xRadar = true;
}

void SubspacePlayer::initStatus()
{
	if(isSpectator())
	{
		status_.reset();
		return;
	}
	
	status_.maxEnergy = shipSettings_->MaximumEnergy;
	status_.energy = shipSettings_->InitialEnergy;
	status_.rechargeRate = shipSettings_->InitialRecharge;

	isDead_ = false;

	status_.thrustPower = shipSettings_->InitialThrust;
	currentThrust_ = Vector(0,0,0);

	status_.turnRate = shipSettings_->InitialRotation;
	currentTurn_ = 0;
    
	status_.bounty = 0;
	giveMultiprize(shipSettings_->InitialBounty);

}

void SubspacePlayer::initWeapons()
{
	if(isSpectator())
	{
		weapons_.reset();
		return;
	}

	//TODO: make this use 'set()' functions, to accomodate for invalid inputs
	weapons_.guns.level = shipSettings_->InitialGuns;
	weapons_.guns.bouncing = false;
	weapons_.guns.multifire = false;

	weapons_.bombs.emp = shipSettings_->EmpBomb;
	weapons_.bombs.level = shipSettings_->InitialBombs;
	weapons_.bombs.shrapnel = 0;
	weapons_.bombs.bounces = shipSettings_->BombBounceCount;
}

void SubspacePlayer::givePrize(PrizeType prize)
{
	switch(prize)
	{
	case PRIZE_Antiwarp:	upgradeAntiwarp();			break;
	case PRIZE_BBullets:	upgradeBounce();			break;
	case PRIZE_Bombs:		upgradeBombs();				break;
	case PRIZE_Brick:		upgradeBricks();			break;
	case PRIZE_Burst:		upgradeBursts();			break;
	case PRIZE_Cloak:		upgradeCloak();				break;	//aren't allowed to have them,
	case PRIZE_Decoy:		upgradeDecoys();			break;
	case PRIZE_Energy:		upgradeMaxEnergy();			break;
	case PRIZE_EngineShutdown:							break;	//TODO: implement engine shutdowns
	case PRIZE_FullCharge:	upgradeFullCharge();		break;
	case PRIZE_Guns:		upgradeGuns();				break;
	case PRIZE_Multifire:	upgradeMultifire();			break;
	case PRIZE_Multiprize:	giveMultiprize(arenaSettings_->MultiPrizeCount);	break;
	case PRIZE_Portal:		upgradePortals();			break;
	case PRIZE_Proximity:	upgradeProximity();			break;	
	case PRIZE_Recharge:	upgradeRechargeRate();		break;
	case PRIZE_Repel:		upgradeRepels();			break;
	case PRIZE_Rocket:		upgradeRockets();			break;
	case PRIZE_Rotation:	upgradeRotationRate();		break;	
	case PRIZE_Shields:		upgradeShields();			break;
	case PRIZE_Shrapnel:	upgradeShrapnel();			break;
	case PRIZE_Stealth:		upgradeStealth();			break;	
	case PRIZE_Super:		upgradeSuper();				break;	
	case PRIZE_Thor:		upgradeThors();				break;
	case PRIZE_Thruster:	upgradeThruster();			break;
	case PRIZE_TopSpeed:	upgradeTopSpeed();			break;
	case PRIZE_XRadar:		upgradeXRadar();			break;  
	case PRIZE_Warp:		warp();						break;
	case PRIZE_Unknown:									break;
	}

	//subspaceSounds[SOUND_Prize].play();

	//++data_.bounty;
	++status_.bounty;
}

void SubspacePlayer::giveMultiprize(Uint numPrizes)
{
	assert(prizeGenerator_);
	
	for(Uint i=0; i < numPrizes; ++i)
		givePrize(prizeGenerator_->getMultiPrize());
}

void SubspacePlayer::giveFlag(SubspaceFlag* flag)
{
	assert(flag);	//make sure flag is valid

	FlagList::iterator i = std::find(flags_.begin(), flags_.end(), flag);

	if(i != flags_.end())	//already has flag
	{
		debugout << "Warning: " << getName() << " already carrying flag [" << (*i)->getFlagID() << "]" << endl;
		return;
	}

	flags_.push_back(flag);
	++status_.flags;
	//data_.info.flagsCarried = status_.flags;

	flag->setTeam(getTeam());
	flag->setCarrier(this);

	flagTime_ = arenaSettings_->FlagDropDelay * 10.0;	//convert 10ms to ms

	subspaceSounds[SOUND_Flag].play();
}

bool SubspacePlayer::dropFlag(SubspaceFlag* flag)
{
	bool retval = false;
	FlagList::iterator i = std::find(flags_.begin(), flags_.end(), flag);
	
	if(i != flags_.end())
	{
		flags_.erase(i);
		--status_.flags;

		flag->setTeam(getTeam());
		flag->setCarrier(0);
		retval = true;
	}

	return retval;
}

void SubspacePlayer::dropFlags()
{
	SubspaceFlag* flag = 0;
	while(flags_.size() > 0)
	{
		flag = flags_.back();
		dropFlag(flag);
		/*flags_.pop_back();
		--status_.flags;

		flag->setCarrier(0);*/
	}
	flagTime_ = 0;

	//return flag;
}

void SubspacePlayer::giveBall(SubspaceBall* ball)
{
	assert(ball);

	balls_.push_back(ball);
	++status_.balls;

	ball->setOwnerID(getId());
	ball->setIsCarried(true);
	ball->setFriction(shipSettings_->SoccerBallFriction);
}

SubspaceBall* SubspacePlayer::dropBall()
{
	SubspaceBall* ball = 0;
	if(getBalls() > 0)
	{
		ball = balls_.back();
		balls_.pop_back();
		--status_.balls;

		ball->setIsCarried(false);
		ball->setFriction(shipSettings_->SoccerBallFriction);
	}

	return ball;
}

bool SubspacePlayer::hasKoth() const
{
	return (kothTime_ > 0);
}

void SubspacePlayer::setKothTime(double milliseconds)
{
	kothTime_ = milliseconds;
}

double SubspacePlayer::getKothTime() const
{
	return kothTime_;
}

void SubspacePlayer::changeShip(ShipType ship)
{
	//data_.info.shipType = ship;
	status_.ship = ship;

	debugout << "Ship change: " << ship << endl;
	//debugout.setBaseStream(&std::cout);
	//printf("Ship Change: %d\n", ship);
	//debugout.write(printfString("Ship change: %d", ship));
	//debugout.flush();

	initShip();
}

void SubspacePlayer::changeTeam(int team, ShipType ship)
{
	//data_.info.team = team;
	status_.team = team;

	changeShip(ship);
	
	/*data_.info.shipType = ship;
	status_.ship = ship;

	initShip();*/
}

void SubspacePlayer::die(bool dead)
{
	isDead_ = dead;

	if(isDead_)
		++data_.losses;
}

void SubspacePlayer::killedPlayer(Uint playerID, Uint bounty)
{
	++data_.wins;

	data_.killPoints += bounty;
}

void SubspacePlayer::rechargeEnergy(double time)
{
	status_.energy += status_.rechargeRate * time / 10000;

	if(status_.energy > status_.maxEnergy)
		status_.energy = status_.maxEnergy;
}

void SubspacePlayer::applyDamage(double damage)
{
	if(isSafe())	//no damage when safe
		return;

	double dmg = damage;
	if(getFlags() > 0)
		dmg *= arenaSettings_->FlaggerDamagePercent/1000.0;

	status_.energy -= dmg;

	if(status_.energy < 0)
	{
		status_.energy = 0;
		//zone_->playerDeath(this);
	}
}

/*********************
 * Weapon Management *
 *********************/

void SubspacePlayer::fireWeapon(const WeaponData& weapon)
{
	//Make sure weapon states are set before calling this
	
	//TODO: check if empty weapon packet = level 0 gun
	switch(weapon.type)
	{
		case WEP_None:
			break;
		case WEP_Bullet:
			weapons_.guns.bouncing = false;
			weapons_.guns.level = weapon.level+1;
			weapons_.guns.multifire = weapon.alternate;

			if(weapon.alternate)
				fireMultiBullet();
			else
				fireBullet();
			break;
		case WEP_BouncingBullet:
			weapons_.guns.bouncing = true;
			weapons_.guns.level = weapon.level+1;
			weapons_.guns.multifire = weapon.alternate;
			
			if(weapon.alternate)
				fireMultiBullet();
			else
				fireBullet();
			break;
		case WEP_ProximityBomb:
			items_.proximity = true;

			if(weapon.bouncing)
				weapons_.bombs.bounces = shipSettings_->BombBounceCount;
			weapons_.bombs.emp = shipSettings_->EmpBomb != 0;//weapon.emp;
			weapons_.bombs.level = weapon.level+1;
			weapons_.bombs.shrapnel = weapon.shrapCount;
			
			if(weapon.bouncing)
				weapons_.guns.bouncing = true;

					//TODO: how do you really get shrapnel level?
			if(!weapon.isBomb)
				weapons_.guns.level = 1;			

			if(weapon.alternate)
				fireMine();
			else
				fireBomb();
			break;
		case WEP_Bomb:
			items_.proximity = false;

			if(weapon.bouncing)
				weapons_.bombs.bounces = shipSettings_->BombBounceCount;
			weapons_.bombs.emp = shipSettings_->EmpBomb != 0;//weapon.emp;
			weapons_.bombs.level = max(weapon.level+1, 3);
			weapons_.bombs.shrapnel = weapon.shrapCount;
			
			if(weapon.bouncing)
				weapons_.guns.bouncing = true;

					//TODO: how do you really get shrapnel level?
			if(!weapon.isBomb)
				weapons_.guns.level = 1;

			if(weapon.alternate)
				fireMine();
			else
				fireBomb();
			break;
		case WEP_Decoy:

			fireDecoy();
			break;
		
		case WEP_Repel:
			
			fireRepel();
			break;
		case WEP_Thor:
			
			fireThor();
			break;
		default:

			debugout << "Unknown weapon fired: " << weapon.type << endl;
			//printf("Unknown weapon fired: %d\n", weapon.type);
	}
	/*WEP_None,
	WEP_Bullet,
	WEP_BouncingBullet,
	WEP_Bomb,
	WEP_ProximityBomb,
	WEP_Repel,
	WEP_Decoy,
	WEP_Burst,
	WEP_Thor,
	WEP_Unknown*/
}

SubspaceBullet* SubspacePlayer::fireBullet()
{
	if(this == zone_->getMyPlayer())
	{
		if(this->isSafe())
		{
			this->velocity_.clear();
			return 0;
		}		

		if(weapons_.guns.multifire)
			return fireMultiBullet();

		if(bulletFireDelay_ > 0)
			return 0;

		if(weapons_.guns.level == 0)	//no guns available
			return 0;	
	}

	//create new bullet
	Uint gunLevel = weapons_.guns.level;
	if(isMyPlayer() && getFlags() > 0)
		++gunLevel;

	SubspaceBullet* b = new SubspaceBullet(gunLevel, weapons_.guns.bouncing);

	b->setOwner(this);
	b->setZone(this->zone_);
	//b->setSubspaceDirection(this->getSubspaceDirection());
	b->setDirection(this->getDirection());
	b->setPosition(this->getPosition());
	b->setVelocity(this->getVelocity() + this->getDirection()*shipSettings_->BulletSpeed);
	b->setTimeToLive(arenaSettings_->BulletAliveTime*10.0);	//BulletAliveTime/100=seconds, *10 is ms

	/*if(weapons_.guns.multifire)
        bulletFireDelay_ = shipSettings_->MultiFireDelay*10.0;
	else*/
		bulletFireDelay_ = shipSettings_->BulletFireDelay*10.0;

	bombFireDelay_ = shipSettings_->BulletFireDelay*10.0;		//wait for bullets to finish

	zone_->addBullet(b);


	//add sound
	subspaceSounds[SOUND_Gun1 + gunLevel-1].play();

	return b;
}

/*SubspaceBrick* SubspacePlayer::fireBrick()
{
	if(this == zone_->getMyPlayer())
	{
		if(bombFireDelay_ > 0 || bulletFireDelay_ > 0)
			return 0;
	}

	//create new brick
	SubspaceBrick* b = new SubspaceBrick();
	
	b->setOwner(this);
	b->setZone(this->zone_);
	//b->setDirection(this->getDirection());
	b->setPosition(this->getPosition());
	b->setTimeToLive(arenaSettings_->BrickTime*10);		

	bombFireDelay_ = shipSettings_->BombFireDelay*10;
	bulletFireDelay_ = shipSettings_->BombFireDelay*10;

	zone_->addBrick(b);

	return b;
}*/

SubspaceBomb* SubspacePlayer::fireBomb()
{
	if(this == zone_->getMyPlayer())
	{
		if(bombFireDelay_ > 0)
			return 0;

		if(weapons_.bombs.level == 0)	//no bombs available
			return 0;
	}

	//create new bomb
	Uint bombLevel = weapons_.bombs.level;
	if(isMyPlayer() && getFlags() > 0)
		++bombLevel;

	SubspaceBomb* b = new SubspaceBomb(bombLevel, weapons_.bombs.shrapnel, weapons_.bombs.bounces, weapons_.bombs.emp, 0, weapons_.guns.level, weapons_.guns.bouncing);
		
	b->setOwner(this);
	b->setZone(this->zone_);
	b->setDirection(this->getDirection());
	b->setPosition(this->getPosition());
	b->setVelocity(this->getVelocity() + this->getDirection()*shipSettings_->BombSpeed);
	b->setTimeToLive(arenaSettings_->BombAliveTime * 10.0);		//BombAliveTime/100 = seconds

	b->setTexture();	

	if(items_.proximity)
	{
		double range = (arenaSettings_->ProximityDistance + weapons_.bombs.level) * SubspaceMap::tileWidth;
		b->setProximityRange(range);
	}

	if(!weapons_.bombs.emp)								//emp bombs have no bullet delay
		bulletFireDelay_ = shipSettings_->BombFireDelay*10.0;

	bombFireDelay_ = shipSettings_->BombFireDelay*10.0;	//delay in 10 ms, e.g. 150= 150 * 10ms = 1.5s

	//TODO: move this adding into the zone itself
	zone_->addBomb(b);

	//recoil
	//currentThrust_ += shipSettings_->BombThrust * -getDirection().unitVector();
	if(this == zone_->getMyPlayer())
		applyThrust(shipSettings_->BombThrust, true);

	if(b->isEmp())
		subspaceSounds[SOUND_EBomb1 + bombLevel-1].play();
	else
		subspaceSounds[SOUND_Bomb1 + bombLevel-1].play();

	return b;
}

SubspaceBurstBullet* SubspacePlayer::fireBurst()
{
	if(this == zone_->getMyPlayer())
	{
		if(bombFireDelay_ > 0)
			return 0;

		if(items_.bursts == 0)	//no bursts available
			return 0;
		else
		{
			--items_.bursts;
		}
	}

	//create new burst
	double angleInc = 2*pi / shipSettings_->BurstShrapnel;
	double angle = 0;
	double speed = shipSettings_->BurstSpeed;
	SubspaceBurstBullet* b = 0;
	for(Uint i=0; i < shipSettings_->BurstShrapnel; ++i)
	{
		b = new SubspaceBurstBullet();

		b->setOwner(this);
		b->setZone(this->zone_);
		b->setDirection(this->getDirection());
		b->setPosition(this->getPosition());

		Vector dir(cos(angle), sin(angle), 0);
		b->setVelocity(dir * speed);

		b->setTimeToLive(arenaSettings_->BulletAliveTime * 10.0);		//BombAliveTime/100 = seconds

		//TODO: move this adding into the zone itself?
		zone_->addBullet(b);

		angle += angleInc;
	}

	bombFireDelay_ = shipSettings_->BombFireDelay*10.0;	//delay in 10 ms, e.g. 150= 150 * 10ms = 1.5s
	bulletFireDelay_ = bombFireDelay_;

	subspaceSounds[SOUND_Burst].play();

	return b;
}

SubspaceDecoy* SubspacePlayer::fireDecoy()
{
	if(this == zone_->getMyPlayer())
	{
		if(bombFireDelay_ > 0)
			return 0;

		if(items_.decoys == 0)	//no decoys available
			return 0;
		else
		{
			--items_.decoys;
		}
	}

	//create new decoy
	SubspaceDecoy* d = new SubspaceDecoy(this);
	
	d->setOwner(this);
	d->setZone(this->zone_);
	//d->setDirection(this->getDirection());	//direction handled by image
	d->setPosition(this->getPosition());
	d->setVelocity(this->getVelocity());
	d->setTimeToLive(arenaSettings_->DecoyAliveTime*10.0);		//DecoyAliveTime/100 = seconds

	bulletFireDelay_ = shipSettings_->BombFireDelay*10.0;	//TODO: check delays for firing item

	bombFireDelay_ = shipSettings_->BombFireDelay*10.0;	//delay in 10 ms, e.g. 150= 150 * 10ms = 1.5s

	zone_->addWeapon(d);

	subspaceSounds[SOUND_Decoy].play();

	return d;
}

SubspaceBomb* SubspacePlayer::fireMine()
{
	if(this == zone_->getMyPlayer())
	{
		if(bombFireDelay_ > 0)
			return 0;

		if(weapons_.bombs.level == 0)	//no bombs available
			return 0;

		if(mines_ >= shipSettings_->MaxMines)
			return 0;
	}

	//create new bomb
	SubspaceBomb* b = new SubspaceBomb(weapons_.bombs.level, weapons_.bombs.shrapnel, weapons_.bombs.bounces, weapons_.bombs.emp, 0, weapons_.guns.level, weapons_.guns.bouncing, true);
	
	b->setOwner(this);
	b->setZone(this->zone_);
	//b->setSubspaceDirection(this->getSubspaceDirection());
	b->setDirection(Vector(0,0,0));
	b->setPosition(this->getPosition());
	b->setVelocity(Vector(0,0,0));
	b->setTimeToLive(arenaSettings_->MineAliveTime * 10.0);		//BombAliveTime/100 = seconds

	b->setTexture();

	if(items_.proximity)
	{
		double range = (arenaSettings_->ProximityDistance + weapons_.bombs.level) * SubspaceMap::tileWidth;
		b->setProximityRange(range);
	}

	
	bulletFireDelay_ = shipSettings_->LandmineFireDelay*10.0;
	bombFireDelay_ = shipSettings_->LandmineFireDelay*10.0;	//delay in 10 ms, e.g. 150= 150 * 10ms = 1.5s

	mineFired();
	//TODO: move this adding into the zone itself
	zone_->addBomb(b);

	subspaceSounds[SOUND_Mine1 + weapons_.bombs.level-1].play();

	return b;
}


SubspaceMultiBullet* SubspacePlayer::fireMultiBullet()
{
	if(this == zone_->getMyPlayer())
	{
		if(bulletFireDelay_ > 0)
			return 0;

		if(weapons_.guns.level == 0)	//no guns available
			return 0;	

		if(items_.multifire == 0)	//no multifire available
			return 0;	
	}

	//create new bullets
	
	double fireAngle = shipSettings_->MultiFireAngle / 111.0f;	//degrees 
	double angleInc = degreesToRadians(fireAngle);
    double speed = shipSettings_->BulletSpeed;
	double angle = -angleInc;
	double baseAngle = atan2(getDirection().y_, getDirection().x_);

	SubspaceMultiBullet* b[3];
	for(Uint i=0; i < 3; ++i)
	{
		b[i] = new SubspaceMultiBullet(weapons_.guns.level, weapons_.guns.bouncing);
	
		b[i]->setOwner(this);
		b[i]->setZone(this->zone_);
		b[i]->setDirection(this->getDirection());
		b[i]->setPosition(this->getPosition());

		Vector dir(cos(baseAngle + angle), sin(baseAngle + angle), 0);
		b[i]->setVelocity(this->getVelocity() + dir*speed);

		b[i]->setTimeToLive(arenaSettings_->BulletAliveTime*10.0);		//BombAliveTime/100 = seconds

		//TODO: move this adding into the zone itself?
		zone_->addBullet(b[i]);

		angle += angleInc;
	}

	bulletFireDelay_ = shipSettings_->MultiFireDelay*10.0;
	bombFireDelay_ = shipSettings_->MultiFireDelay*10.0;		//wait for bullets to finish

	b[0]->addChild(b[1]);
	b[0]->addChild(b[2]);

	b[1]->setParent(b[0]);
	b[2]->setParent(b[0]);

	//TODO: add separate sound for multifire bullet
	subspaceSounds[SOUND_Gun1 + weapons_.guns.level-1].play();

	return b[0];
}

SubspaceRepel* SubspacePlayer::fireRepel()
{
	if(this == zone_->getMyPlayer())
	{
		if(bulletFireDelay_ > 0 || bombFireDelay_ > 0)
			return 0;

		if(items_.repels == 0)	//no repels available
			return 0;	
		else
		{
			--items_.repels;
		}
	}

	//create new repel
	SubspaceRepel* r = new SubspaceRepel();
	
	r->setOwner(this);
	r->setZone(this->zone_);
	r->setDirection(this->getDirection());
	r->setPosition(this->getPosition());
	r->setVelocity(Vector(0,0,0));
	r->setTimeToLive(arenaSettings_->RepelTime * 10.0);

	bulletFireDelay_ = shipSettings_->BombFireDelay * 10.0;
	bombFireDelay_ = shipSettings_->BombFireDelay * 10.0;		//wait for bullets to finish

	//zone adds repel targets
	zone_->addRepel(r);

	subspaceSounds[SOUND_Repel].play();

	return r;
}


SubspaceThor* SubspacePlayer::fireThor()
{
	if(this == zone_->getMyPlayer())
	{
		//only do checks for my player
		if(bombFireDelay_ > 0)
			return 0;

		if(items_.thors == 0)	//no thors available
			return 0;
		else
		{
			--items_.thors;
		}
	}

	//create new thor
	SubspaceThor* t = new SubspaceThor();
	
	t->setOwner(this);
	t->setZone(this->zone_);
	//b->setSubspaceDirection(this->getSubspaceDirection());
	t->setDirection(this->getDirection());
	t->setPosition(this->getPosition());
	t->setVelocity(this->getVelocity() + this->getDirection()*shipSettings_->BombSpeed);
	t->setTimeToLive(arenaSettings_->BombAliveTime*10.0);		//BombAliveTime/100 = seconds

	bulletFireDelay_ = shipSettings_->BombFireDelay*10.0;

	bombFireDelay_ = shipSettings_->BombFireDelay*10.0;	//delay in 10 ms, e.g. 150= 150 * 10ms = 1.5s

	zone_->addBomb(t);

	if(this == zone_->getMyPlayer())
		applyThrust(shipSettings_->BombThrust, true);

	subspaceSounds[SOUND_Thor].play();

	return t;
}

SubspacePortal* SubspacePlayer::firePortal()
{
	if(this == zone_->getMyPlayer())
	{
		if(bulletFireDelay_ > 0 || bombFireDelay_ > 0)
			return 0;

		if(items_.portals == 0)	//no portals available
			return 0;	
		else
		{
			--items_.portals;
		}
	}

	//create new portal
	SubspacePortal* p = new SubspacePortal();
	
	p->setOwner(this);
	p->setZone(this->zone_);
	p->setDirection(Vector(0,0,0));
	p->setPosition(this->getPosition());
	p->setVelocity(Vector(0,0,0));
	p->setTimeToLive(arenaSettings_->WarpPointDelay * 10.0);

	//portalPosition_ = this->getPosition();
	activePortal_ = p;
	isPortalActive_ = true;

	bulletFireDelay_ = shipSettings_->BombFireDelay * 10.0;
	bombFireDelay_ = shipSettings_->BombFireDelay * 10.0;

	subspaceSounds[SOUND_WarpPoint].play();

	return p;
}

void SubspacePlayer::mineDestroyed()
{
	assert(mines_ > 0);

	--mines_;
}

void SubspacePlayer::mineFired()
{
	debugout << "Mine fired: " << mines_ << endl;
	//printf("Mine fired: %d\n", mines_);

	//no check - possible for a bunch of bombs to be converted to mines
	++mines_;
}

bool SubspacePlayer::useBrick()
{
	if(bombFireDelay_ > 0)
		return false;

	if(items_.bricks > 0)
	{
		--items_.bricks;
		bombFireDelay_ = shipSettings_->BombFireDelay*10;
		bulletFireDelay_ = shipSettings_->BombFireDelay*10;

		subspaceSounds[SOUND_Wall].play();
	}

	return true;
}

bool SubspacePlayer::hasMoreMines() const
{
	return (mines_ < shipSettings_->MaxMines);
}

bool SubspacePlayer::isPortalActive() const
{
	return isPortalActive_;
}

bool SubspacePlayer::usePortal()
{
	bool retval = false;
	if(isPortalActive_)
	{
		assert(activePortal_);

		createFlash();
		//setPosition(portalPosition_);
		setPosition(activePortal_->getPosition());
		status_.flashing = true;
		//zone_->forceSendPosition();
		
		createFlash();

		activePortal_ = 0;
		isPortalActive_ = false;
		retval = true;

		zone_->clearPortals(this);

		subspaceSounds[SOUND_Warp].play();
	}
	
	return retval;
}

bool SubspacePlayer::useRocket()
{
	if(items_.rockets <= 0)
		return false;

	if(rocketTime_ > 0)
		return false;

	--items_.rockets;
	rocketTime_ = shipSettings_->RocketTime * 10.0;
	
	subspaceSounds[SOUND_Rocket1].play(true);
	subspaceSounds[SOUND_Rocket2].play(true);

	return true;
}

void SubspacePlayer::clearPortal()
{
	activePortal_ = 0;
	isPortalActive_ = false;
}

double SubspacePlayer::getPortalTime() const
{
	double retval = 0;

	if(isPortalActive_)
	{
		assert(activePortal_);

		retval = activePortal_->getTimeToLive();
	}

	return retval;
}

double SubspacePlayer::getShieldsTime() const
{
	return (double)items_.shields;
}

double SubspacePlayer::getShieldsPercent() const
{
	return (double)items_.shields/shipSettings_->ShieldsTime;
}

double SubspacePlayer::getSuperTime() const
{
	return (double)items_.superPower;
}

/***********************
 * Position Management *
 ***********************/

void SubspacePlayer::applyThrust(double power, bool reverse)
{
	double time = 1;	//apply for 10 ms
	if(reverse)
	{
		applyForce(power * -direction_, time);
	}
	else
	{
		applyForce(power * direction_, time);
	}
}

void SubspacePlayer::thrust(bool maxThrust)
{
	if(isThrusting_)	//already applied thrust for this timestep
		return;

	if(isTurret())
		return;

	if(isSpectator())		//special case for spectators
	{
		if(maxThrust)
			//move(Vector(0, 20, 0));
			moveAccum_ += Vector(0, SPECTATOR_MOVE_RATE_FAST, 0);
		else
			//move(Vector(0, 10, 0));
			moveAccum_ += Vector(0, SPECTATOR_MOVE_RATE, 0);
		isThrusting_ = true;
		isAfterburner_ = false;
		return;
	}

	double power;
	if(maxThrust)
	{
		power = shipSettings_->MaximumThrust;
		isAfterburner_ = true;
		isThrusting_ = false;

		subspaceSounds[SOUND_Thrust].setLoop(true);
		subspaceSounds[SOUND_Thrust].play(true);
	}
	else
	{
		isAfterburner_ = false;
		power = status_.thrustPower;

		subspaceSounds[SOUND_Rev].setLoop(true);
		subspaceSounds[SOUND_Rev].play(true);
	}

	if(getFlags() > 0)
		power +=  arenaSettings_->FlaggerThrustAdjustment;

	if(rocketTime_ > 0)
	{
		power = (double)arenaSettings_->RocketThrust;
		isAfterburner_ = false;
	}

	currentThrust_ += power * this->direction_;
	isThrusting_ = true;

	if(exhaustDisplayDelay_ <= 0)
	{
		if(rocketTime_ > 0)
			createExhaust(true, 3, degreesToRadians(90.0), true);
		else
			createExhaust(true, 2, degreesToRadians(60.0), false);
		exhaustDisplayDelay_ = maxExhaustDisplayDelay;
	}
}

void SubspacePlayer::thrustReverse(bool maxThrust)
{
	if(isThrusting_)
		return;

	if(isTurret())
		return;

	if(rocketTime_ > 0)		//can't reverse thrust while rocketing
		return;

	if(isSpectator())		//special case for spectators
	{
		if(maxThrust)
			//move(Vector(0, -20, 0));
			moveAccum_ += Vector(0, -SPECTATOR_MOVE_RATE_FAST, 0);
		else
			//move(Vector(0,- 10, 0));
			moveAccum_ += Vector(0, -SPECTATOR_MOVE_RATE, 0);
		isThrusting_ = true;
		isAfterburner_ = false;
		return;
	}

	double power;
	if(maxThrust)
	{
		power = shipSettings_->MaximumThrust;
		isAfterburner_ = true;
		isThrusting_ = false;

		//subspaceSounds[SOUND_Thrust].setLoop(true);
		subspaceSounds[SOUND_Thrust].play(true);
	}
	else
	{
		isAfterburner_ = false;
		power = status_.thrustPower;

		//subspaceSounds[SOUND_Rev].setLoop(true);
		subspaceSounds[SOUND_Rev].play(true);
	}

	currentThrust_ -= power * this->direction_;
	isThrusting_ = true;

	if(exhaustDisplayDelay_ <= 0)
	{
		createExhaust(false, 2, degreesToRadians(60.0), false);
		exhaustDisplayDelay_ = maxExhaustDisplayDelay;
	}
}

void SubspacePlayer::turnLeft(bool maxTurn)
{
	if(isTurning_)
		return;

	if(isSpectator())		//special case for spectators
	{
		if(maxTurn)
			//move(Vector(-20, 0, 0));
			moveAccum_ += Vector(-SPECTATOR_MOVE_RATE_FAST, 0, 0);
		else
			//move(Vector(-10, 0, 0));
			moveAccum_ += Vector(-SPECTATOR_MOVE_RATE, 0, 0);
	}
	else
	{
		currentTurn_ += status_.turnRate;
	}

	isTurning_ = true;
}

void SubspacePlayer::turnRight(bool maxTurn)
{
	if(isTurning_)
		return;

	if(isSpectator())		//special case for spectators
	{
		if(maxTurn)
			//move(Vector(20, 0, 0));
			moveAccum_ += Vector(SPECTATOR_MOVE_RATE_FAST, 0, 0);
		else
			//move(Vector(10, 0, 0));
			moveAccum_ += Vector(SPECTATOR_MOVE_RATE, 0, 0);
	}
	else
	{
		currentTurn_ -= status_.turnRate;
	}

	isTurning_ = true;
}

void SubspacePlayer::warp(bool activated)
{
	warp(SubspaceMap::maxTileX / 2, SubspaceMap::maxTileY / 2, activated);
}

void SubspacePlayer::warp(int centerX, int centerY, bool activated)
{
	if(isSpectator())
		return;

	int randX = centerX;	//TODO: should be max tile coordinates from map - huh?
	int randY = centerY;
	int warpRadiusLimit = arenaSettings_->WarpRadiusLimit;

	if(warpRadiusLimit)		//can't % by 0
	{
		randX += (rand() % warpRadiusLimit) - warpRadiusLimit/2;
		randY += (rand() % warpRadiusLimit) - warpRadiusLimit/2;
	}
	randX *= 16;
	randY *= 16;

	createFlash();

	this->setSubspacePosition(Vector((double)randX, (double)randY, 0));
	this->setSubspaceVelocity(Vector(0, 0, 0));

	status_.flashing = true;
	//zone_->forceSendPosition();

	if(activated)
	{
		status_.energy = 0;
		subspaceSounds[SOUND_Warp].play();
	}
}

void SubspacePlayer::move(const Vector& dir)
{
	position_ += dir;
}

/**********************
 * Physics Management *
 **********************/
Rect SubspacePlayer::getBoundingBox() const
{
	double size = getSize()/2.0;
	Rect retval(position_.y_+size, position_.x_-size, position_.y_-size, position_.x_+size);
	
	return retval;
}

/*******************
 * Special Effects *
 *******************/
void SubspacePlayer::createExhaust(bool reverse, Uint num, double angle, bool rocket)
{
	double thrustSpeed = 1.5;
	
	Vector dir = getDirection();
	double startAngle = atan2(dir.unitVector().y_, dir.unitVector().x_);
	double baseAngle;
	if(reverse)
		baseAngle = atan2(-dir.unitVector().y_, -dir.unitVector().x_);
	else
		baseAngle = startAngle;

	double currAngle = -angle / 2.0;
	
	Vector start;
	double radius = getSize()/2.0 - 2;
	if(rocket)
		radius = getSize()/2.0;

	for(Uint i=0; i < num; ++i)
	{
		currAngle += angle / (double)(num+1);

		if(reverse)
		{
			dir.x_ = cos(baseAngle - currAngle);
			dir.y_ = sin(baseAngle - currAngle);
		}
		else
		{
			dir.x_ = cos(currAngle + baseAngle);
			dir.y_ = sin(currAngle + baseAngle);
		}

		start.x_ = cos(startAngle - currAngle);
		start.y_ = sin(startAngle - currAngle);

		SubspaceExhaust* exhaust = new SubspaceExhaust(rocket);
		exhaust->setPosition(getPosition() - start*radius);
		exhaust->setVelocity(getVelocity() + dir * 1000 * thrustSpeed);
		zone_->addEffect(exhaust, true);
	}
}

void SubspacePlayer::createFlash()
{
	SubspaceFlash* flash = new SubspaceFlash();
	flash->setPosition(this->getPosition());
	zone_->addEffect(flash);
}

/***********
 * Turrets *
 ***********/

SubspacePlayer* SubspacePlayer::getTurretParent() const
{
	return turretParent_;
}

const SubspacePlayerList& SubspacePlayer::getTurrets() const
{
	return turrets_;
}

void SubspacePlayer::addTurret(SubspacePlayer* p)
{
	SubspacePlayerList::iterator i = std::find(turrets_.begin(), turrets_.end(), p);

	if(i != turrets_.end())	//player already exists as turret
		return;

	turrets_.push_back(p);	
}

void SubspacePlayer::clearTurrets()
{
	SubspacePlayerList::iterator i;
	for(i = turrets_.begin(); i != turrets_.end(); ++i)
	{
		SubspacePlayer* p = (*i);

		p->setTurretParent(0);
	}

	turrets_.clear();
}

void SubspacePlayer::removeTurret(SubspacePlayer* p)
{
	SubspacePlayerList::iterator i = std::find(turrets_.begin(), turrets_.end(), p);		//TODO: use turrets_.remove() ???

	if(i == turrets_.end())	//player is not turreted
		return;

	turrets_.erase(i);
}

void SubspacePlayer::setTurretParent(SubspacePlayer* p)
{
	turretParent_ = p;

	setTexture();
}

/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////

void SubspacePlayer::update(double t)
{
	this->getTexture()->setFrameInRange(this->getSubspaceDirection());

	//update weapons
	if(status_.flashing)
	{
		createFlash();
		//this->data_.state.flash = 0;
		status_.flashing = false;
	}
	bulletFireDelay_ -= t;	//time in milliseconds
	bombFireDelay_ -= t;	//time in milliseconds

	//turn ship
	this->rotateZ(currentTurn_ * t/1000.0);	//why do i need *10? rotation in rads/sec, game in 1/100ms
	currentTurn_ = 0;
	isTurning_ = false;

	//sound
	if(isMyPlayer())
	{		
		subspaceSounds[SOUND_Hum].setLoop(true);
		if(!subspaceSounds[SOUND_Hum].isPlaying() && !isSpectator())
			subspaceSounds[SOUND_Hum].play(true);
		if(isSpectator())							//stop playing in spectator
			subspaceSounds[SOUND_Hum].stop();

		Sound::setListenerPosition(position_);		// TODO: make constants for the sound multipliers
		Sound::setListenerVelocity(velocity_/100.0);	
	}

	//TODO: add energy reduction from afterburners
	//apply thrust
	if(rocketTime_ > 0)
	{
		rocketTime_ -= t;
		thrust();

		if(!subspaceSounds[SOUND_Rocket1].isPlaying())
		{
			subspaceSounds[SOUND_Rocket2].setLoop(true);
			subspaceSounds[SOUND_Rocket2].play(true);
		}
	}
	else
	{
		subspaceSounds[SOUND_Rocket1].stop();
		subspaceSounds[SOUND_Rocket2].stop();
	}

	if(!isThrusting_)
    	subspaceSounds[SOUND_Rev].stop();
		
	if(!isAfterburner_)		
		subspaceSounds[SOUND_Thrust].stop();
	
	this->velocity_ += currentThrust_ * t/10.0;	//thrust in pixels/sec, t in /100ms
	currentThrust_ = Vector(0,0,0);
	isThrusting_ = false;	
	isAfterburner_ = false;

	exhaustDisplayDelay_ -= t;
	if(exhaustDisplayDelay_ < 0)
		exhaustDisplayDelay_ = 0;

	//check max speed
	double maxSpeed = shipSettings_->MaximumSpeed;
	if(getFlags() > 0)
		maxSpeed += arenaSettings_->FlaggerSpeedAdjustment;
	if(rocketTime_ > 0)
		maxSpeed = (double)arenaSettings_->RocketSpeed;

	double currentSpeed = (this->velocity_ + currentThrust_).magnitude();
	if(currentSpeed > maxSpeed)
	{
		this->velocity_ *= maxSpeed / currentSpeed;
	}

	//items
	items_.superPower -= (Uint)t;
	if(items_.superPower < 0)
		items_.superPower = 0;

	items_.shields -= (Uint)t;
	if(items_.shields < 0)
		items_.shields = 0;

	//recharge energy
	rechargeEnergy(t);
	
	//update position, with map collision
	Vector newPosition, newVelocity;
												//TODO: make this use actual ship size
	Rect rect(position_.y_+15, position_.x_-15, position_.y_-15, position_.x_+15);
	Rect newRect(0, 0, 0, 0);
	double friction = 16.0 / arenaSettings_->BounceFactor;
	if(zone_->handleMapBoxCollision(t, rect, getVelocity(), newRect, newVelocity, friction))
	{
		position_.x_ = newRect.left + 15;
		position_.y_ = newRect.bottom + 15;
		velocity_ = newVelocity;
	}
	else
	{
		SubspaceGameObject::update(t);
	}

	position_ += moveAccum_ * t;
	moveAccum_.clear();

	if(isTurret())
	{
		position_ = turretParent_->getPosition();
		velocity_ = turretParent_->getVelocity();
	}

	//check safety
	Uint tx, ty;
	objectToTileCoords(position_, &tx, &ty);
	if(zone_->getMap() && zone_->getMap()->getTile(tx, ty).isSafe())
		setSafe(true);
	else
		setSafe(false);

	//update balls
	//TODO: add ball and flag drop timers - added in display, only need ball timers now
	BallList::const_iterator b;
	for(b = balls_.begin(); b != balls_.end(); ++b)
	{
		SubspaceBall* ball = (*b);
		ball->setPosition(getPosition());
	}

	//update flags
	if(flagTime_ && getFlags() > 0)		//can drop flags from time
	{					
		flagTime_ -= t;
		if(flagTime_ < 0)
			zone_->playerDropFlag(getId(), true);
	}

	//king of the hill
	if(kothTime_ > 0)
	{
		kothTime_ -= t;
		if(kothTime_ < 0)
			zone_->playerKothEnded(getId());
	}


	safeTime_ -= t;
	if(safeTime_ < 0)
		safeTime_ = 0;
}


void SubspacePlayer::draw() const
{
	//draw ball
	if(getBalls() > 0)
	{
		SubspaceBall* b = balls_.front();
		b->draw();
	}

	if(isDead())
		return;

	//draw player
	SubspaceGameObject::draw();

	//draw turrets

	//draw border
	/*glDisable(GL_TEXTURE_2D);
	glColor4f(1, 1, 1, 1);
	glLineWidth(1);
	int size = getSize()/2;
	glBegin(GL_LINE_LOOP);
		glVertex2f(-size, -size);
		glVertex2f(size, -size);
		glVertex2f(size, size);
		glVertex2f(-size, size);
		glVertex2f(-size, -size);
	glEnd();

	double epsilon = 0.1;

	//draw border
	double x = position_.x_ - floor(clamp(position_.x_/(double)SubspaceMap::tileWidth, 0.1))*SubspaceMap::tileWidth;
		
	double y = position_.y_ - floor(clamp(position_.y_/(double)SubspaceMap::tileHeight, 0.1))*SubspaceMap::tileHeight;
	
	double startXf = (position_.x_-getSize()/2.0) / SubspaceMap::tileWidth;
	double endXf = (position_.x_+getSize()/2.0) / SubspaceMap::tileWidth;

	startXf = MathUtil::clamp(startXf, epsilon);
	endXf = MathUtil::clamp(endXf, epsilon);

	int startX = startXf;
    int endX = ceil(endXf);

	//has problems with negative tiles from here
	int widthR = (int)((endX - startX + 1)/2) * SubspaceMap::tileWidth;
	int widthL = (endX - startX)/2 * SubspaceMap::tileWidth;
	double width = (endX - startX) * SubspaceMap::tileWidth;

	double startYf = (position_.y_-getSize()/2.0) / SubspaceMap::tileHeight;
	double endYf = (position_.y_+getSize()/2.0) / SubspaceMap::tileHeight;

	startYf = MathUtil::clamp(startYf, epsilon);
	endYf = MathUtil::clamp(endYf, epsilon);

	int startY = startYf;
	int endY = ceil(endYf);

	double heightL = (int)((endY - startY + 1)/2) * SubspaceMap::tileHeight;
	double heightR = (endY - startY)/2 * SubspaceMap::tileHeight;
	double height = (endY - startY) * SubspaceMap::tileHeight;

	glBegin(GL_LINE_LOOP);
		glVertex2f(0, 0);
		glVertex2f(width, 0);
	glEnd();


	glDisable(GL_TEXTURE_2D);
	glColor4f(1, 0, 0, 1);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-x-widthL, -y-heightR);
		glVertex2f(-x+widthR, -y-heightR);
		glVertex2f(-x+widthR, -y+heightL);
		glVertex2f(-x-widthL, -y+heightL);
		glVertex2f(-x-widthL, -y-heightR);
	glEnd();

	glColor4f(1, 1, 1, 1);*/
}

void SubspacePlayer::display() const
{
	if(isDead())
		return;

	if(isSpectator())
		return;

	//Sprite::display();
	glPushMatrix();
		glTranslated(getPosition().x_, getPosition().y_, getPosition().z_);

		//glRotatef(90, 0, 0, 1);	//rotation by animation frames
		
		//TODO use quaternions or something for rotation - yes quaternions
		//double angle = atan2f(this->getDirection().y_, this->getDirection().x_);	//direction should be a unit vector
        //glRotatef(angle, 0, 0, 1);

		this->draw();
	glPopMatrix();
}

void SubspacePlayer::setTexture()
{
	TextureAnimated tex;
	if(isTurret())
	{
		if(isMyPlayer())
		{
			tex.load("graphics/turret.bm2", false);
			tex.setFrameSize(40, 40);
		}
		else
		{
			tex.load("graphics/turret2.bm2", false);
			tex.setFrameSize(16, 16);
		}
	}
	else
	{
		tex.load("graphics/ships.bm2", false);
		tex.setFrameSize(36, 36);
		tex.setFrameRange(0, 40);

		Uint shipType = status_.ship;
        tex.setFrameRange(40*shipType, 40*shipType + 40);
	}

	Sprite::setTexture(tex);
}