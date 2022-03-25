//David Millman

#ifndef _SUBSPACEGAMEOBJECT_H_
#define _SUBSPACEGAMEOBJECT_H_

#include "Sprite.h"
#include "GameObject.h"

class SubspaceGameObject : 
	public Sprite			//TODO: necessary to be a sprite?
	//public GameObject
{
public:
	SubspaceGameObject();
	~SubspaceGameObject();

	void setDirection(const Vector& direction);
	void setSubspaceDirection(int direction);
	int getSubspaceDirection() const;

	void setSubspacePosition(const Vector& position);
	void setSubspaceVelocity(const Vector& velocity);
	const Vector& getSubspacePosition() const;
	const Vector& getSubspaceVelocity() const;

	const Vector& getNextSubspacePosition(double time) const;

	static void subspacePositionToStandardPosition(Vector& pos);	//changes position
	static void standardPositionToSubspacePosition(Vector& pos);	//changes position

	static void subspaceVelocityToStandardVelocity(Vector& vel);	//changes position

	Uint getTileX() const;
	Uint getTileY() const;

	Uint getSubspaceTileX() const;
	Uint getSubspaceTileY() const;

	void update(double time);	//time in milliseconds

private:

	//int subspaceDir_;
};

#endif