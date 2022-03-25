//David Millman

#ifndef _SUBSPACEBOMB_H_
#define _SUBSPACEBOMB_H_

#include "SubspaceWeapon.h"
#include "TextureAnimated.h"

class SubspacePlayer;

class SubspaceBomb :
	public SubspaceWeapon
{
public:
	SubspaceBomb(int level = 0, int shrapnel = 0, int bouncing = 0, bool emp = false, double proxRange = 0, Uint shrapLevel = 0, bool shrapBounce = false, bool isMine = false);
	virtual ~SubspaceBomb();

	WeaponData getWeaponSettings() const;	

	//TODO: add proximity trigger delay
	//TODO: make tile based range instead? - that's how cont does it i think
	void addTarget(SubspacePlayer* target);		//TODO: make this an addtarget list
	void setProximityRange(double range);
	bool isProximityActive() const;

	double getDamage(double distance = 0) const;
	int getLevel() const;
	Uint getShrapnel() const;
	bool getShrapnelBounce() const;
	Uint getShrapnelLevel() const;

	void setLevel(int level);
	void setBouncing(int bouncing);
	void setEmp(bool isEmp);

	void setTexture();

	bool isBouncy() const;
	bool isEmp() const;
	bool isMine() const;
	void destroy();

	void update(double timeMultiplier);

	void draw() const;

private:
	struct ProximityTarget
	{
		double distance;
		SubspacePlayer* object;
	};

	typedef list<ProximityTarget> TargetList;
	TargetList targets_;

	bool isProximityActive_;
	double proximityRange_;

	int level_;
	Uint shrapnel_;
	Uint shrapnelLevel_;
	int bouncing_;
	bool emp_;	
	bool shrapnelBounce_;
	bool isMine_;

protected:
	TextureAnimated bombTexture_, mineTexture_;
};

#endif