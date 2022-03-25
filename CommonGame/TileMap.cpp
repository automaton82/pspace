#include "TileMap.h"

TileMap::TileMap(double tileWidth, double tileHeight) :
	width_(0), height_(0),
	tileWidth_(tileWidth), tileHeight_(tileHeight)
{
	//init(width_, height_);
}

TileMap::~TileMap()
{
}

bool TileMap::load(TileType* tiles, Uint width, Uint height)
{
	Uint index, i, j;

	init(width, height);

    for(i=0; i < height; ++i)
	{
		for(j=0; j < width; ++j)
		{
			index = i*width_ + j;
			tiles_[index] = tiles[index];
		}
	}

	return true;
}

Uint TileMap::getWidth() const
{
	return width_;
}

Uint TileMap::getHeight() const
{
	return height_;
}

double TileMap::getTileWidth() const
{
	return tileWidth_;
}

double TileMap::getTileHeight() const
{
	return tileHeight_;
}

void TileMap::setTileSize(double tileWidth, double tileHeight)
{
	tileWidth_ = tileWidth;
	tileHeight_ = tileHeight;
}

void TileMap::setTileTexture(const TextureAnimated& texture)
{
	tilesTexture_ = texture;
}

void TileMap::objectToTileCoords(const Vector& p, Uint* tileX, Uint* tileY) const
{
	*tileX = (Uint)(p.x_ / tileWidth_);
	*tileY = (Uint)(p.y_ / tileHeight_);
}


void TileMap::tileToObjectCoords(Uint tileX, Uint tileY, Vector* p) const
{
	p->x_ = tileX * tileWidth_;
	p->y_ = tileY * tileHeight_;
}


bool TileMap::isTileCollidable(Uint x, Uint y) const
{
	assert(x <= width_ && y <= height_);

	//return (tiles_[y*width_ + x] != 0);

	if(tiles_[y*width_ + x] > 1)
		return true;
	else
		return false;
}

void TileMap::setDrawRange(Uint x, Uint y, Uint xEnd, Uint yEnd)
{
	assert(x <= xEnd && y <= yEnd);

	x = min(x, width_);
	y = min(y, height_);

	xEnd = min(xEnd, width_);
	yEnd = min(yEnd, height_);

	drawX_ = x;
	drawY_ = y;
	drawXEnd_ = xEnd;
	drawYEnd_ = yEnd;
}

void TileMap::display() const
{
	glPushMatrix();
		glTranslated(drawX_*tileWidth_, drawY_*tileHeight_, 0);
		drawTileList(tiles_, drawX_, drawY_, drawXEnd_, drawYEnd_);
	glPopMatrix();
}

void TileMap::init(Uint width, Uint height)
{
	tiles_.resize(width*height, 0);		//initialize all to 0

	width_ = width;
	height_ = height;
}

void TileMap::drawTile(TileType type) const
{
	if(!type)
		return;

	glPushMatrix();
		glScaled(tileWidth_ / tilesTexture_.getFrameWidth(), tileHeight_ / tilesTexture_.getFrameHeight(), 1);
		tilesTexture_.displayFrame(type - 1);
	glPopMatrix();

	//tilesTexture_.displayFrame(type - 1);
}

void TileMap::drawTileList(const TileList& list, Uint drawX, Uint drawY, Uint drawXEnd, Uint drawYEnd) const
{
	Uint r, c;
	Uint drawWidth = drawXEnd - drawX;

	for(r = drawY; r < drawYEnd; ++r)
	{
		for(c = drawX; c < drawXEnd; ++c)
		{
			drawTile(list[r*width_ + c]);
			glTranslated(tileWidth_, 0, 0);
		}
		glTranslated(-(double)drawWidth*tileWidth_, tileHeight_, 0);
	}
}