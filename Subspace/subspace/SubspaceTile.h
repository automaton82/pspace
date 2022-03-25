//David Millman

#ifndef _SUBSPACETILE_H_
#define _SUBSPACETILE_H_

#include <string>
using std::string;

#include "Updatable.h"

#include "TextureAnimated.h"

class SubspaceTile
{
public:
	typedef Uchar TileType;

public:
	SubspaceTile();
	SubspaceTile(int x, int y, TileType type);
	~SubspaceTile();

	SubspaceTile(const SubspaceTile& copy);
	SubspaceTile& operator=(const SubspaceTile& rhs);

	//data
	int getX() const;
	int getY() const;

	TileType getType() const;
	void setType(TileType type);	//sets up animated frames

	bool isAsteroid() const;
		bool isAsteroidPart() const;
		bool isBigAsteroid() const;
	bool isBrick() const;
	bool isCollidable() const;	//specific types only
	bool isDoor() const;
	bool isEmpty() const;		//specific tile type - nothing in it
	bool isGoal() const;
		bool isTeamGoal() const;
		bool isEnemyGoal() const;
	bool isNormal() const;		
	bool isOver() const;
	bool isSafe() const;
	bool isUnder() const;
	bool isWormhole() const;

	//drawing
	static void initTextures();
	static void setTilesTexture(const TextureAnimated& texture);
	static void drawTile();

    void draw() const;	//drawing handled by map, takes too much memory for individual textures

	//update animations
	void update(double time);
	static void updateTextures(double time);
		
public:
	int x_, y_;		//TODO: are these actually needed?
	TileType type_;

	TextureAnimated* texture_;	//TODO: need this dynamically created?
								//TODO: use global animated texture for all tiles of same type?
	//static TextureAnimated doors_;
	//static TextureAnimated asteroids_;

	static TextureAnimated tilesTexture_;	//texture for all tiles
	static TextureAnimated doorHorizontalTexture_;
	static TextureAnimated doorVerticalTexture_;
	static TextureAnimated asteroidSmall1Texture_;
	static TextureAnimated asteroidSmall2Texture_;
	static TextureAnimated asteroidLargeTexture_;
	static TextureAnimated stationTexture_;
	static TextureAnimated wormholeTexture_;
	static TextureAnimated goalEnemyTexture_;
	static TextureAnimated goalTeamTexture_;
	
public:

	//TODO: is there some way to put an enum in another file? - and still inside class
	//FROM MERVBOT
	/* VIE map format

	* X	= WORD((N >> 12) & 0xFFF);	// This puts actual map dimensions at 4095x4095 possible tiles.
	* Y	= WORD(N & 0xFFF);
	* TILE = BYTE(v >> 24);

	* 0		= No tile
	* 1-19		= Normal tiles
	* 20		= Border
	* 21-161	= Normal tiles
	* 162-165	= Vertical doors
	* 166-169	= Horizontal doors
	* 170		= Flag
	* 171		= Safe zone
	* 172		= Goal area
	* 173-175	= Fly over tiles
	* 176-191	= Fly under tiles

	* Warning: Deviating from this format may invalidate the security checksum.

	*/

	enum ExtendedTileType
	{
		vieNoTile,			// These are VIE constants

		vieNormalStart		= 1,
		vieBorder			= 20,	// Borders are not included in the .lvl files
		vieNormalEnd		= 161,	// Tiles up to this point are part of sec.chk

		vieDoorStart		= 162,
		vieVDoorStart		= 162,
		vieVDoorEnd			= 165,

		vieHDoorStart		= 166,
		vieHDoorEnd			= 169,
		vieDoorEnd			= 169,

		vieSpecialStart		= 170,

		vieTurfFlag			= 170,

		vieSafeZone			= 171,	// Also included in sec.chk

		vieGoalArea			= 172,

		vieFlyOverStart		= 173,
		vieFlyOverEnd		= 175,
		vieFlyUnderStart	= 176,
		vieFlyUnderEnd		= 190,

		vieSpecialEnd		= 190,

		vieAsteroidStart	= 216,
		vieBigAsteroid		= 217,
		vieAsteroidEnd		= 218,

		vieStation			= 219,

		vieWormhole			= 220,

		ssbTeamBrick,				// These are internal, ssb=subspace bot ?
		ssbEnemyBrick,

		ssbTeamGoal,
		ssbEnemyGoal,

		ssbTeamFlag,
		ssbEnemyFlag,

		ssbPrize,

		ssbAsteroidPart,			//inivisible asteroid
		ssbBorder					// Use ssbBorder instead of vieBorder to fill border
	};
};

#endif