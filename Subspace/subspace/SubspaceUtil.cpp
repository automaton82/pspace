#include "SubspaceUtil.h"

class SubspaceMap;
#include "SubspaceMap.h"

#include "MathUtil.h"
using namespace MathUtil;

#include "SubspaceGlobal.h"
#include "SubspacePlayer.h"

const double TOLERANCE = 0.001;			//collision tolerance

void tileToObjectCoords(Uint x, Uint y, Vector* p)
{
	p->x_ = x * SubspaceMap::tileWidth;
	p->y_ = y * SubspaceMap::tileHeight;
}


void objectToTileCoords(const Vector& p, Uint* x, Uint* y)
{
	*x = (Uint)(p.x_ / SubspaceMap::tileWidth);
	*y = (Uint)(p.y_ / SubspaceMap::tileHeight);
}


void objectToTileCoords(const Vector& p, int* x, int* y)
{
	*x = (int)(p.x_ / SubspaceMap::tileWidth);
	*y = (int)(p.y_ / SubspaceMap::tileHeight);

	if(p.x_ < 0) --(*x);				// (int)-0.9 is still 0, should be tile -1 though
	if(p.y_ < 0) --(*y);
}

static Vector camera;
void setScreenCamera(const Vector& c)
{
	camera = c;
}

void objectToScreenCoords(const Vector& p, Vector* screen)
{
	screen->x_ = p.x_ - camera.x_;
	screen->y_ = p.y_ - camera.y_;
}

bool isOnScreen(const Vector& p)
{
	Vector screen;
	objectToScreenCoords(p, &screen);

	if(screen.x_ >= 0 && screen.x_ <= subspaceGlobal.getWindowWidth() &&
		screen.y_ >= 0 && screen.y_ <= subspaceGlobal.getWindowHeight())
		return true;
	else
		return false;
}

int getPlayerID(const SubspacePlayerMap& players, const string& name)
{
	SubspacePlayerMap::const_iterator i;
	for(i = players.begin(); i != players.end(); ++i)
	{
		if(name == (*i).second->getName())
			break;
	}

	if(i != players.end())
		return (*i).second->getId();
	else
		return INVALID_PLAYER_ID;
}