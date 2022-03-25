#ifndef _SUBSPACEMAP_H_
#define _SUBSPACEMAP_H_

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

#include "Bitmap.h"
#include "TextureAnimated.h"

#include "SubspaceTile.h"
#include "SubspaceGameObject.h"

class SubspaceMap
	//public TileMap,	//TODO: make this a type of tileMap? - 
						//TODO: YES MAKE GENERIC COMPONENT
						//TODO: ESPECIALLY FOR COLLISION DETECTION - WHICH WORKS!		
{
public:
	const static int maxTileX = 1024, maxTileY = 1024;
	const static int tileWidth = 16, tileHeight = 16;

private:
	typedef map<int, SubspaceGameObject*> ObjectMap;

public:
	typedef vector< vector<SubspaceTile::TileType> > BasicMatrix;

	typedef map<int, SubspaceTile> SubspaceTileMap;	

	typedef vector<SubspaceTile> TileList;
	typedef vector<TileList> TileMatrix;	
	typedef map<Uint, TextureAnimated> AnimatedTileMap;	//global tile animations

public:
	SubspaceMap();
	~SubspaceMap();

	//loading
	bool load(const string& filename);			//TODO: store map as sparse matrix? or quad-tree? oct-tree for expansion?
	bool isLoaded() const;

	//data
	int getWidth() const;
	int getHeight() const;
	const string& getMapFilename() const;

	const TextureAnimated& getTilesTexture() const;

					//TODO: add border tiles
	const SubspaceTile& getTile(int x, int y) const;	//0,0 is upper left
	const BasicMatrix& getTileMatrix() const;

	//TODO: implement doors and goals
	//TODO: fix up bricks
	//TODO: enemy bricks are not correct, fix them

	bool setBrick(int x, int y, bool myTeam = false);		//note: display for bricks not handled
	bool clearBrick(int x, int y);

	void setGoalTeam(int team);
	void updateGoals();
	
	//map related objects - prizes and flags
	bool addObject(int tileX, int tileY, SubspaceGameObject* object, bool takeNextTile = false);
	SubspaceGameObject* getTileObject(int tileX, int tileY);
	SubspaceGameObject* removeObject(SubspaceGameObject* object);

	//drawing
	void setDrawRange(int x, int y, int xEnd, int yEnd);
	void draw() const;			//draws all tiles, shouldn't really be used
		
	void drawStandardTiles() const;
	void drawOverTiles() const;
	void drawUnderTiles() const;	

	//update
	void update(double time);	//update doors and animate tiles	

private:

	void drawBorderTiles() const;
	void drawObject(const SubspaceGameObject& gameObject) const;
	void drawObjectMap(const ObjectMap& map) const;
	void drawTile(const SubspaceTile& tile) const;
	void drawTileMap(const SubspaceTileMap& map) const;
	void drawTileMap2(const SubspaceTileMap& map) const;
	void drawTileMap3(const SubspaceTileMap& map) const;
	
	void init();
	void initBorderTiles();
	void insertTile(const SubspaceTile& tile);

	//static bool isBorderTile(int x, int y);
	bool isInDrawRange(int tx, int ty) const;

	bool readBitmapInfo(HANDLE file, Bitmap* bitmap, Uchar*& data);
	bool readTileInfo(HANDLE file, int& validTiles, int& invalidTiles);

	static int toLinear(int x, int y);
	static int toLinearBorder(int x, int y);

	template <class T>	void updateList(T& list, double time);
	void updateObjectMap(ObjectMap& map, double time);	
	void updateSubspaceTileMap(SubspaceTileMap& map, double time);	

private:
	BasicMatrix tiles_;		//for collision detection and specific tile access
	int width_, height_;
	
	// Borders
	SubspaceTileMap borderTiles_;
	int borderType_;

	//display and animation
	int drawX_, drawY_, drawXEnd_, drawYEnd_;
	TextureData texData_;
	TextureAnimated texture_;
	AnimatedTileMap animatedTextures_;		// TODO: global textures for faster updates

	//goals
	int goalTeam_;
	
	//map objects - prizes, flags, bricks
	ObjectMap objects_;

	// TODO: make these vector< map<SubspaceTile> >, it's not exactly a sparse matrix
	// tiles
	SubspaceTileMap drawTiles_;			// non-animated tiles
	SubspaceTileMap animatedTiles_;		// does not include door tiles - is this needed at all?
	SubspaceTileMap asteroidTiles_;		
	SubspaceTileMap doorTiles_;
	SubspaceTileMap goalTiles_;
	SubspaceTileMap overTiles_;
	SubspaceTileMap underTiles_;
	SubspaceTileMap wormholeTiles_;		// TODO: finish implementing this

	// map loading
	string mapFilename_;
	bool isLoaded_;
};

#endif