#include "Tile.h"

#include <gl/glaux.h>
#include "GameDisplayManager.h"

Tile::Tile()
{
}

Tile::~Tile()
{
}

void Tile::update(double timeMultiplier)
{
}

void Tile::draw() const
{
	
}

bool Tile::isCollidable() const
{
	//check tile types here
	if(this->type_ != 0)
		return true;
	else
		return false;
}