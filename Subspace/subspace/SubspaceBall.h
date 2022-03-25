//David Millman

#ifndef _SUBSPACEBALL_H_
#define _SUBSPACEBALL_H_

#include "SubspaceGameObject.h"
#include "TextureAnimated.h"

class SubspaceBall :
	public SubspaceGameObject
{
public:
	SubspaceBall(Uint ballID = 0);
	~SubspaceBall();

	Uint getBallID() const;
	void setBallID(Uint id);

	void setIsCarried(bool isCarried);
	bool isCarried() const;

	void setOwnerID(Uint id);	//current/last person to carry ball
	Uint getOwnerID() const;

	void setFriction(double friction);

	Uint getTimestamp() const;
	void setTimestamp(Uint timestamp);	//last update time

	//TODO: figure out how friction is used to calculate ball position
	//update		
	void update(double time);

	//draw
	void draw() const;
	void display() const;

private:

	Uint ballID_;
	bool isCarried_;
	Uint ownerID_;

	double friction_;
	Uint timestamp_;

	TextureAnimated ballTexture_;
};

#endif