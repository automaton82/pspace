// Subspace weapon and game object stubs for interim gameplay
#include "DataTypes.h"
#include "Vector.h"
#include "Rect.h"

// Forward declarations to avoid include dependencies
class SubspacePlayer;
class SubspaceZone;
class SubspaceGameObject;
struct WeaponData;

// SubspaceWeapon stubs (minimal class definition)
class SubspaceWeapon {
public:
    void setTimeToLive(double ttl) { }
    void setOwner(SubspacePlayer* owner) { }
    void setZone(SubspaceZone* zone) { }
    SubspacePlayer* getOwner() const { return nullptr; }
    SubspaceZone* getZone() const { return nullptr; }
    double getTimeToLive() const { return 0.0; }
};

// SubspacePlayer upgrade stub methods (forward declared in headers)
namespace SubspacePlayerStubs {
    void upgradeMultifire() { }
    void upgradePortals(unsigned int count) { }
    void upgradeRockets(unsigned int count) { }
    void upgradeBricks(unsigned int count) { }
    void upgradeThors(unsigned int count) { }
    void upgradeDecoys(unsigned int count) { }
    void upgradeBursts(unsigned int count) { }
    void upgradeRepels(unsigned int count) { }
    void upgradeAntiwarp() { }
    void upgradeShrapnel() { }
    void upgradeShields() { }
    void upgradeSuper() { }
    void upgradeProximity() { }
    void upgradeFullCharge() { }
    void upgradeTopSpeed() { }
    void upgradeThruster() { }
    void upgradeBounce() { }
    void upgradeBombs() { }
    void upgradeGuns() { }
    void upgradeXRadar() { }
    void upgradeCloak() { }
    void upgradeStealth() { }
    void upgradeRotationRate() { }
    void upgradeMaxEnergy() { }
    void upgradeRechargeRate() { }
    bool isSafe() { return true; }
    void setSafe(bool safe) { }
}

// SubspaceZone network stub methods
namespace SubspaceZoneStubs {
    void sendPosition() { }
    void sendWeapon(const WeaponData& data) { }
    void sendFlagsDrop() { }
    void handleMapObjectCollisions() { }
    bool handleMapBoxCollision(double time, const RectangleType<double>& rect, 
        const Vector& velocity, RectangleType<double>& newRect, Vector& newVelocity, 
        double bounce, bool checkSafe) { return false; }
}

// SubspaceTile stub methods  
namespace SubspaceTileStubs {
    bool isDoor() { return false; }
    bool isTeamGoal() { return false; }
    bool isGoal() { return false; }
    bool isSafe() { return true; }
    bool isCollidable() { return false; }
    bool isAsteroid() { return false; }
    bool isAsteroidPart() { return false; }
}
