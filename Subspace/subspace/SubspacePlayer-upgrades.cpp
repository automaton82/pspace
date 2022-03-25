//David Millman

#include "SubspacePlayer.h"

void SubspacePlayer::upgradeAntiwarp()
{
	if(shipSettings_->AntiwarpStatus != 0)
		items_.antiwarp = true;
}

void SubspacePlayer::upgradeBombs()
{
	if(weapons_.bombs.level + 1 <= shipSettings_->MaxBombs) 
		++weapons_.bombs.level;
}

void SubspacePlayer::upgradeBounce()
{
	weapons_.guns.bouncing = true;
}

void SubspacePlayer::upgradeBricks(Uint num)
{
	items_.bricks += num;

	if(items_.bricks > shipSettings_->BrickMax)
		items_.bricks = shipSettings_->BrickMax;
}

void SubspacePlayer::upgradeBursts(Uint num)
{
	items_.bursts += num;

	if(items_.bursts > shipSettings_->BurstMax)
		items_.bursts = shipSettings_->BurstMax;
}

void SubspacePlayer::upgradeCloak()
{
	if(shipSettings_->CloakStatus != 0)
		items_.cloak = true;
}

void SubspacePlayer::upgradeDecoys(Uint num)
{
	items_.decoys += num;

	if(items_.decoys > shipSettings_->DecoyMax)
		items_.decoys = shipSettings_->DecoyMax;
}

void SubspacePlayer::upgradeFullCharge()
{
	//data_.energy = (Uint16)status_.maxEnergy;
	status_.energy = status_.maxEnergy;
}

void SubspacePlayer::upgradeGuns()
{
	if(weapons_.guns.level + 1 <= shipSettings_->MaxGuns)
		++weapons_.guns.level;
}

void SubspacePlayer::upgradeMaxEnergy()
{
	status_.maxEnergy += shipSettings_->UpgradeEnergy;

	if(status_.maxEnergy > shipSettings_->MaximumEnergy)
		status_.maxEnergy = shipSettings_->MaximumEnergy;
}

void SubspacePlayer::upgradeMultifire()
{
	items_.multifire = true;
}

void SubspacePlayer::upgradePortals(Uint num)
{
	items_.portals += num;

	if(items_.portals > shipSettings_->PortalMax)
		items_.portals = shipSettings_->PortalMax;
}

void SubspacePlayer::upgradeProximity()
{
	items_.proximity = true;
}

void SubspacePlayer::upgradeRechargeRate()
{
	status_.rechargeRate += shipSettings_->UpgradeRecharge;

	if(status_.rechargeRate > shipSettings_->MaximumRecharge)
		status_.rechargeRate = shipSettings_->MaximumRecharge;
}

void SubspacePlayer::upgradeRepels(Uint num)
{
	items_.repels += num;

	if(items_.repels > shipSettings_->RepelMax)
		items_.repels = shipSettings_->RepelMax;
}

void SubspacePlayer::upgradeRockets(Uint num)
{
	items_.rockets += num;

	if(items_.rockets > shipSettings_->RocketMax)
		items_.rockets = shipSettings_->RocketMax;
}

void SubspacePlayer::upgradeRotationRate()
{
	status_.turnRate += shipSettings_->UpgradeRotation;

	if(status_.turnRate > shipSettings_->MaximumRotation)
		status_.turnRate = shipSettings_->MaximumRotation;
}

void SubspacePlayer::upgradeShields()
{
	items_.shields = shipSettings_->ShieldsTime * 10;	//hundreths of a second to milliseconds
}

void SubspacePlayer::upgradeStealth()
{
	if(shipSettings_->StealthStatus != 0)
		items_.stealth = true;
}

void SubspacePlayer::upgradeShrapnel()
{
	weapons_.bombs.shrapnel += shipSettings_->ShrapnelRate;

	if(weapons_.bombs.shrapnel > shipSettings_->ShrapnelMax)
		weapons_.bombs.shrapnel = shipSettings_->ShrapnelMax;
}

void SubspacePlayer::upgradeSuper()
{
	items_.superPower = shipSettings_->SuperTime * 10;	//hundreths of a second to milliseconds
}

void SubspacePlayer::upgradeThors(Uint num)
{
	items_.thors += num;

	if(items_.thors > shipSettings_->ThorMax)
		items_.thors = shipSettings_->ThorMax;
}

void SubspacePlayer::upgradeThruster()
{
	status_.thrustPower += shipSettings_->UpgradeThrust;

	if(status_.thrustPower > shipSettings_->MaximumThrust)
		status_.thrustPower = shipSettings_->MaximumThrust;
}

void SubspacePlayer::upgradeTopSpeed()
{
	status_.topSpeed += shipSettings_->UpgradeSpeed;

	if(status_.topSpeed > shipSettings_->MaximumSpeed)
		status_.topSpeed = shipSettings_->MaximumSpeed;
}

void SubspacePlayer::upgradeXRadar()
{
	if(shipSettings_->XRadarStatus != 0)
		items_.xRadar = true;
}
