//David Millman

#include "DataTypes.h"
#include "GameObject.h"

#ifndef _TILE_H_
#define _TILE_H_

class Tile : 
	public GameObject
{
public:
	Tile();
	~Tile();

	void update(double timeMultiplier);

	void draw() const;

	//Collidable Interface
	bool isCollidable() const;

private:

	int type_;
};

#endif