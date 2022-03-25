//David Millman

#ifndef _SUBSPACEFLAG_H_
#define _SUBSPACEFLAG_H_

#include "SubspaceGameObject.h"
#include "TextureAnimated.h"

class SubspacePlayer;

class SubspaceFlag :
	public SubspaceGameObject
{
public:
	SubspaceFlag(Uint flagID = 0);
	~SubspaceFlag();

	SubspacePlayer* getCarrier() const;
	Uint getFlagID() const;
	Uint getTeam() const;

	void setCarrier(SubspacePlayer* player);
	void setFlagID(Uint id);
	void setIsActive(bool b);
	void setIsMyTeamFlag(bool b);
	void setTeam(Uint team);
	
	bool isActive() const;
	bool isCarried() const;
	bool isMyTeamFlag() const;
	
	//update
	void update(double time);
	
	//draw
	void draw() const;
	//void display() const;

private:

	void updateTexture();

private:

	Uint flagID_;
	Uint team_;

	bool isActive_;		//inactive when a network player requests the flag
	bool isMyTeamFlag_;
						//TODO: add inactive timer

	SubspacePlayer* carrier_;
	//SubspacePlayer* lastCarrier_;

	TextureAnimated flagTexture_;
};

#endif