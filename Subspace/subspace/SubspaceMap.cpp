//David Millman

#include "SubspaceMap.h"

#include <algorithm>
#include <fstream>

#include "BmpObject.h"
#include "MathUtil.h"
#include "SubspaceGlobal.h"

SubspaceMap::SubspaceMap() : 
	borderType_(20),
	isLoaded_(false),
	goalTeam_(-1)
{
	init();
}

SubspaceMap::~SubspaceMap()
{
}

bool SubspaceMap::load(const string& filename)
{	
	//Bitmap* bitmap = 0;
	Bitmap bitmap;
	BmpObject bmpObj;
	bool useDefaultTile = false;

	std::ifstream file(filename.c_str());

	if(!file.is_open())
	{
		//this->log("File not found: %s", filename.c_str());
		debugout << "File not found: " << filename << endl;
		return false;
	}
	file.close();

	//this->log("Reading file \"%s\"...", filename.c_str());
	debugout << "Loading mapfile \"" << AsciiUtil::trim(filename) << "\"..." << endl;

	
	if(!bitmap.loadFromFile(filename))	//uses bitmap just to check
	//!bmpObj.load(filename))	//if no bitmap data
	{
		//this->log("No tile bitmap found.");
		debugout << "No tile bitmap found, loading default..." << ends;
		useDefaultTile = true;

		if(!bmpObj.load("graphics/tiles.bm2"))	//load default data
		{
			debugout << "Error loading default bitmap data." << endl;
			//this->log("Error Loading default bitmap data.");
			return false;
		}
	}
	else
	{
		bmpObj.load(filename);
	}

	texData_.setColorKey(Color(0,0,0));
	texData_.loadFromMemory(bmpObj.getData(), bmpObj.getWidth(), bmpObj.getHeight(), false);

	this->texture_.setTextureData(&texData_);
	texture_.setFrameSize(tileWidth, tileHeight);
	texture_.setFrameRange(0, 190);
	SubspaceTile::setTilesTexture(texture_);
	SubspaceTile::initTextures();
	
	//Load tile data
	HANDLE fileHandle = CreateFile(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	
	if(!useDefaultTile)
	{
		bitmap.loadFromHandle(fileHandle);	//move past bitmap
	}
	
	//TODO: this should use HBITMAP and LoadImage, maybe?

	//initialize map size;
	width_ = maxTileX;
	height_ = maxTileY;

	tiles_.clear();
	tiles_.resize(height_);

	BasicMatrix::iterator i;
	for(i = tiles_.begin(); i != tiles_.end(); ++i)
	{
		vector<SubspaceTile::TileType>& row = (*i);
		row.resize(width_);
	}

	int validTiles, invalidTiles;

	//this->log("Reading tile data...");
	debugout << "Reading tile data..." << ends;
	if(!readTileInfo(fileHandle, validTiles, invalidTiles))
	{
		debugout << "Error reading tile data." << endl;
		//this->log("Error reading tile data.");
		isLoaded_ = false;
	}
	else
	{
		debugout << "done." << endl;
		isLoaded_ = true;
	}

	if(invalidTiles != 0)
	{
		debugout << "Found " << validTiles << " valid tiles, " << invalidTiles << " invalid tiles." << endl;
		//this->log("Found %d valid tiles, %d invalid tiles.", validTiles, invalidTiles);
	}

	CloseHandle(fileHandle);

	/*if(bitmap)
		delete bitmap;*/

	initBorderTiles();

	if(isLoaded_)
	{
		debugout << "Map loaded successfully." << endl;
		//this->log("Map loaded successfully.");

		mapFilename_ = filename;
	}

	/*std::ofstream test("map2.txt");
	SubspaceTileMap::iterator t, s, x;

	
	for(t = drawTiles_.lower_bound(toLinear(500, 500)); (*t).first < toLinear(1, 700) && t != drawTiles_.end(); )	
	{

		for(s = t; (*s).first < toLinear(500 + 400, (*t).second.y_); ++s)	
		{
			test << "(" << (*s).second.x_ << ", " << (*s).second.y_ << ")" << std::endl;
		}
		t = drawTiles_.lower_bound(toLinear(500, (*t).second.y_+1));				
	}
	test.close();*/

	return isLoaded_;
}

bool SubspaceMap::isLoaded() const
{
	return isLoaded_;
}

int SubspaceMap::getWidth() const
{
	return width_;
}

int SubspaceMap::getHeight() const
{
	return height_;
}

const string& SubspaceMap::getMapFilename() const
{
	return mapFilename_;
}

const TextureAnimated& SubspaceMap::getTilesTexture() const
{
	return texture_;
}

const SubspaceTile& SubspaceMap::getTile(int x, int y) const
{
	static SubspaceTile t;
	if(x >= 0 && x < width_ && y >= 0 && y < height_)
	{
		t.type_ = tiles_[y][x];
	}
	else
	{
		// is border tile?
		int iX = x, iY = y;
		if((iX == -1 || iX == (int)width_+1) && (iY >= -1 && iY < (int)height_+1) ||
			(iY == -1 || iY == (int)height_+1) && (iX >= -1 && iX < (int)width_+1))
		{
			t.type_ = borderType_;
		}
		else
		{
			//t = SubspaceTile(x, y, 0);
			t.type_ = 0;
		}
	}

	t.x_ = x;
	t.y_ = y;

	return t;
}

const SubspaceMap::BasicMatrix& SubspaceMap::getTileMatrix() const
{
	return tiles_;
}

bool SubspaceMap::setBrick(int x, int y, bool myTeam)
{
	bool retval = false;
	Uint index = x * maxTileX + y;
	ObjectMap::iterator i = objects_.find(index);

	if(getTile(x, y).isEmpty() && i == objects_.end())	//empty tile
	{
		if(myTeam)
			tiles_[y][x] = SubspaceTile::ssbTeamBrick;
		else
			tiles_[y][x] = SubspaceTile::ssbEnemyBrick;

		retval = true;
	}

	return retval;
}

bool SubspaceMap::clearBrick(int x, int y)
{
	bool retval = false;

	if(getTile(x, y).isBrick())
	{
		tiles_[y][x] = SubspaceTile::vieNoTile;
		retval = true;
	}

	return retval;
}

void SubspaceMap::setGoalTeam(int team)
{
	if(team == goalTeam_)
		return;
}

bool SubspaceMap::addObject(int tileX, int tileY, SubspaceGameObject* object, bool takeNextTile)
{
	assert(object);
	assert(tileX < maxTileX && tileY < maxTileY);

	bool retval = false;

	Uint index = tileY * maxTileX + tileX;
	ObjectMap::iterator i = objects_.find(index);

	if(getTile(tileX, tileY).isEmpty() && i == objects_.end())
	{
		objects_[index] = object;
		retval = true;
	}

	if(!retval && takeNextTile)	//take closest available tile, spiral outwards
	{
		double radius = 1;
		Uint c = 0;
		double angle = 0;
		Uint nextTileX, nextTileY;

		while(!retval && radius < maxTileX && radius < maxTileX)
		{
			angle += MathUtil::PI/4.0 / radius;

			nextTileX = tileX + radius*cos(angle);
			nextTileY = tileY + radius*sin(angle);
			
			nextTileX = min(maxTileX, nextTileX);
			nextTileY = min(maxTileY, nextTileY);

			index = nextTileY*maxTileX + nextTileX;
			i = objects_.find(index);
			if(getTile(nextTileX, nextTileY).isEmpty() && i == objects_.end())
			{
				objects_[index] = object;
				retval = true;
			}		

			if(angle > 2.0*MathUtil::PI)
			{
				angle = 0;
				++radius;
			}
		}
	}

	return retval;
}

SubspaceGameObject* SubspaceMap::getTileObject(int tileX, int tileY)
{
	SubspaceGameObject* g = 0;

	Uint index = tileY * maxTileX + tileX;
	ObjectMap::iterator i = objects_.find(index);
	if(i != objects_.end())
	{
		g = (*i).second;
		objects_.erase(i);
	}
	
	return g;
}

SubspaceGameObject* SubspaceMap::removeObject(SubspaceGameObject* object)
{
	SubspaceGameObject* g = 0;

	ObjectMap::iterator i;
	for(i = objects_.begin(); i != objects_.end(); ++i)	//TODO: replace with std::find?
	{
		SubspaceGameObject* test = (*i).second;
		if(test == object)
			g = object;
	}
	
	return g;

}

void SubspaceMap::setDrawRange(int x, int y, int xEnd, int yEnd)
{
	assert(x >= -1 && x <= xEnd && y >= -1 && y <= yEnd);

	drawX_ = min( max(-1, x), (int)width_+1);	//clamp at -1 and width+1
	drawY_ = min( max(-1, y), (int)height_+1);

	drawXEnd_ = min( max(-1, xEnd), (int)width_+1);	//clamp at -1 and width+1
	drawYEnd_ = min( max(-1, yEnd), (int)height_+1);
}

void SubspaceMap::draw() const
{
	if(!isLoaded())
		return;
	
	glEnable(GL_TEXTURE_2D);

	drawStandardTiles();
}

void SubspaceMap::drawStandardTiles() const
{
	drawTileMap(asteroidTiles_);
	drawBorderTiles();	// needs separate algorithm for shifting tiles
	drawTileMap(drawTiles_);
	drawTileMap(doorTiles_);

	glColor4d(1.0, 1.0, 1.0, 1.0);	//fix changes from map
	drawTileMap(goalTiles_);
	drawTileMap(wormholeTiles_);

	drawObjectMap(objects_);
}

void SubspaceMap::drawOverTiles() const
{
	drawTileMap(overTiles_);
}

void SubspaceMap::drawUnderTiles() const
{
	drawTileMap(underTiles_);
}

void SubspaceMap::update(double time)
{
	//TODO: update all animation and seed-related things, specifically doors and prizes
	
	/*updateSubspaceTileMap(animatedTiles_, time);
	updateSubspaceTileMap(asteroidTiles_, time);
	updateSubspaceTileMap(doorTiles_, time);
	updateSubspaceTileMap(goalTiles_, time);
	updateSubspaceTileMap(wormholeTiles_, time);*/
	SubspaceTile::updateTextures(time);

	//update door open/close from seeds

	//update prizes
	updateObjectMap(objects_, time);

	//update bricks
}

void SubspaceMap::drawObject(const SubspaceGameObject& g) const
{
	glPushMatrix();
		glTranslated( (g.getPosition().x_/tileWidth-(int)drawX_)*tileWidth,  
			(g.getPosition().y_/tileHeight-(int)drawY_)*tileHeight, 0);
		g.draw();
	glPopMatrix();
}

void SubspaceMap::drawObjectMap(const ObjectMap& objectMap) const
{
	//TODO: assume sparsity? - iterating through all indexes is faster than finding per index
	if(!isLoaded())
		return;

	ObjectMap::const_iterator i;
	for(i = objectMap.begin(); i != objectMap.end(); ++i)
	{
		Uint index = (*i).first;
		Uint x = index % maxTileX;
		Uint y = index / maxTileX;

		if(isInDrawRange(x, y))
		{
		/*if(x >= drawX_ && x <= drawXEnd_ &&
			y >= drawY_ && y <= drawYEnd_)
		{*/
			drawObject( *(*i).second);
		}
	}
}

void SubspaceMap::drawTile(const SubspaceTile& t) const
{
	//int dx = (int)t.x_ - max(0, drawX_);
	//int dy = (int)t.y_ - max(0, drawY_);
	int dx = t.x_ - max(0, drawX_);
	int dy = t.y_ - max(0, drawY_);

	glPushMatrix();
		glTranslated(dx*(double)tileWidth, dy*(double)tileHeight, 0);
		t.draw();
	glPopMatrix();
}

void SubspaceMap::drawTileMap(const SubspaceTileMap& map) const
{
	SubspaceTileMap::const_iterator i, j;
	SubspaceTileMap::const_iterator startCol, startRow;
	int endRowIndex, endColIndex;

	startRow = map.lower_bound(toLinear(drawX_, drawY_));
	endRowIndex = toLinear(drawXEnd_, drawYEnd_);
	for(j = startRow; (*j).first < endRowIndex && j != map.end(); )	
	{
		startCol = j;
		endColIndex = toLinear(drawXEnd_, (*j).second.y_);
		for(i = startCol; (*i).first < endColIndex && i != map.end(); ++i)	
		{
			drawTile((*i).second);			
		}
		j = map.lower_bound(toLinear(drawX_, (*j).second.y_+1));		
	}	
}

void SubspaceMap::drawBorderTiles() const
{
	/*const SubspaceTileMap& map = borderTiles_;

	SubspaceTileMap::const_iterator i, j;
	SubspaceTileMap::const_iterator startCol, startRow;
	int endRowIndex, endColIndex;

	startRow = map.lower_bound(toLinearBorder(drawX_, drawY_));
	endRowIndex = toLinearBorder(drawXEnd_, drawYEnd_);
	for(j = startRow; (*j).first < endRowIndex && j != map.end(); )	
	{
		startCol = j;
		endColIndex = toLinearBorder(drawXEnd_+1, (*j).second.y_);
		for(i = startCol; (*i).first < endColIndex && i != map.end(); ++i)	
		{
			drawTile((*i).second);			
		}
		j = map.lower_bound(toLinearBorder(drawX_, (*j).second.y_+1));		
	}
	const SubspaceTile& s = (*startRow).second;
	const SubspaceTile& e = (*map.upper_bound(endRowIndex)).second;*/

	//printf("dx(%d-%d) dy(%d-%d)  s(%d,%d) e(%d,%d)\n", drawX_, drawY_, drawXEnd_, drawYEnd_, s.x_, s.y_, e.x_, e.y_);
}

void SubspaceMap::init()
{
	tiles_.resize(maxTileY);
	BasicMatrix::iterator i;
	for(i = tiles_.begin(); i != tiles_.end(); ++i)
		(*i).resize(maxTileX);	
}

void SubspaceMap::initBorderTiles()
{
	SubspaceTile b;
	int x, y;

	borderTiles_.clear();

	for(x=-1, y=-1; y <= height_+1; ++y)			//left border
		borderTiles_.insert(pair<int, SubspaceTile>(toLinearBorder(x, y), SubspaceTile(x, y, borderType_)));

	for(x=width_+1, y=-1; y <= height_+1; ++y)		//right border
		borderTiles_.insert(pair<int, SubspaceTile>(toLinearBorder(x, y), SubspaceTile(x, y, borderType_)));

	for(x=-1+1, y=-1; x < width_+1-1; ++x)			//bottom border
		borderTiles_.insert(pair<int, SubspaceTile>(toLinearBorder(x, y), SubspaceTile(x, y, borderType_)));

	for(x=-1+1, y=height_+1; x < width_+1; ++x)		//top border
		borderTiles_.insert(pair<int, SubspaceTile>(toLinearBorder(x, y), SubspaceTile(x, y, borderType_)));
}

void SubspaceMap::insertTile(const SubspaceTile& t)
{	
	if(t.isEmpty())
		return;

	tiles_[t.y_][t.x_] = t.type_;
	int i = toLinear(t.x_, t.y_);

    if(t.isAsteroid())
	{
		asteroidTiles_.insert(std::pair<int, SubspaceTile>(i, t));
		
		if(t.isBigAsteroid())
		{
			tiles_[t.y_]  [t.x_+1] = SubspaceTile::ssbAsteroidPart;
			tiles_[t.y_+1]  [t.x_] = SubspaceTile::ssbAsteroidPart;
			tiles_[t.y_+1][t.x_+1] = SubspaceTile::ssbAsteroidPart;
		}
	}
	else if(t.isDoor())
		doorTiles_.insert(std::pair<int, SubspaceTile>(i, t));
	else if(t.isGoal())
		goalTiles_.insert(std::pair<int, SubspaceTile>(i, t));
	else if(t.isNormal() || t.isSafe())
		drawTiles_.insert(std::pair<int, SubspaceTile>(i, t));
	else if(t.isOver())
		overTiles_.insert(std::pair<int, SubspaceTile>(i, t));
	else if(t.isUnder())
		underTiles_.insert(std::pair<int, SubspaceTile>(i, t));
	else if(t.isWormhole())
		wormholeTiles_.insert(std::pair<int, SubspaceTile>(i, t));
	else
	{
		printf("UNKNOWN TILE TYPE: %d\n", t.type_);
	}
}

bool SubspaceMap::isInDrawRange(int tx, int ty) const
{
	bool retval = false;

	if(tx >= drawX_ && tx <= drawXEnd_ &&
		ty >= drawY_ && ty <= drawYEnd_)
	{
		retval = true;
	}

	return retval;
}

bool SubspaceMap::readBitmapInfo(HANDLE fileHandle, Bitmap* bitmap, Uchar*& data)
{
	bitmap = new Bitmap();

	if(!bitmap->loadFromHandle(fileHandle))
	{
		delete bitmap;
		bitmap = 0;
		return false;
	}
	
	return true;
}

struct TileData
{
	unsigned x : 12;
	unsigned y : 12;
	unsigned type : 8;
};

bool SubspaceMap::readTileInfo(HANDLE fileHandle, int& validTiles, int& invalidTiles)
{
	BOOL readResult;
	unsigned long bytesRead = 0;
	unsigned long currentByte = 0;
		
	invalidTiles = 0;
	validTiles = 0;
	TileData tile;

	drawTiles_.clear();

	do
	{
		readResult = ReadFile(fileHandle, &tile, sizeof(TileData), &bytesRead, NULL);
		if(readResult == FALSE)
		{
			printf("Error reading tile!");
		}
		
		currentByte += bytesRead;
	
		if(tile.x < 0 || tile.x >= maxTileX) 
			invalidTiles++;
		else if(tile.y < 0 || tile.y >= maxTileY)
			invalidTiles++;
		else
		{
			//reverse direction of loading?

			validTiles++;
			
			int realY = maxTileY - tile.y - 1;
			assert(tile.x < width_ && realY < height_);

			SubspaceTile t(tile.x, realY, tile.type);
			insertTile(t);		
		}
	}
	while(bytesRead > 0);

	return true;
}

int SubspaceMap::toLinear(int x, int  y)
{
	x = max(0, min(maxTileX, x));
	y = max(0, min(maxTileY, y));

	return (y * maxTileX + x);	
}
int SubspaceMap::toLinearBorder(int x, int  y)
{
	/*if(x < 0)
		x = 0;
	if(y < 0)
		y = 0;*/
	return (y*(maxTileX+1) + x);	
}

template <class T>
void SubspaceMap::updateList(T& list, double time)
{
	T::iterator i;
	for(i = list.begin(); i != list.end(); ++i)
		(*i).update(time);
}

void SubspaceMap::updateObjectMap(ObjectMap& map, double time)
{
	ObjectMap::iterator i;
	for(i = map.begin(); i != map.end(); ++i)
		(*i).second->update(time);
	
}

void SubspaceMap::updateSubspaceTileMap(SubspaceTileMap& map, double time)
{
	SubspaceTileMap::iterator i;
	for(i = map.begin(); i != map.end(); ++i)
		(*i).second.update(time);
}