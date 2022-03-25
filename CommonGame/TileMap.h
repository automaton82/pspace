#ifndef _TILEMAP_H_
#define _TILEMAP_H_

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "TextureAnimated.h"
#include "Vector.h"

/*
Description:	A set of all basic stuff a map should need - collisions, tile textures, load/save.
				Only has tiles at set x,y increments
*/

//TODO: make dynamic tile map (tiles can be placed anywhere)


class TileMap
{
private:
	typedef Uchar TileType;				//0 - 255 enough?
	typedef vector<TileType> TileList;

public:
	TileMap(double tileWidth = 1.0, double tileHeight = 1.0);
	~TileMap();

	//loading
	bool load(TileType* tiles, Uint width, Uint height);
	//bool load(const string& filename);			//TODO: store map as sparse matrix? or quad-tree? oct-tree for expansion?
	//bool save(const string& filename);
		
	//data
	Uint getWidth() const;
	Uint getHeight() const;

	double getTileWidth() const;
	double getTileHeight() const;

	void setTileSize(double tileWidth, double tileHeight);
	void setTileTexture(const TextureAnimated& texture);

	//utility
	void objectToTileCoords(const Vector& position, Uint* tileX, Uint* tileY) const;
	void tileToObjectCoords(Uint tileX, Uint tileY, Vector* position) const;

	//collision
	virtual bool isTileCollidable(Uint x, Uint y) const;	//assumes non-zero tile is solid - meant to be overriden

	//drawing
	void setDrawRange(Uint x, Uint y, Uint xEnd, Uint yEnd);

	virtual void display() const;				

protected:

	void init(Uint width, Uint height);

	virtual void drawTile(TileType type) const;						//does not translate at all
	virtual void drawTileList(const TileList& list, Uint drawX, Uint drawY, Uint drawXEnd, Uint drawYEnd) const;

protected:
	TileList tiles_;		
	Uint width_, height_;
	double tileWidth_, tileHeight_;	//size of each tile

	Uint drawX_, drawY_, drawXEnd_, drawYEnd_;

	TextureAnimated tilesTexture_;	//texture that contains all tiles
};

#endif