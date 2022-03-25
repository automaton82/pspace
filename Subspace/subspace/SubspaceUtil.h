#ifndef _SUBSPACEUTIL_H_
#define _SUBSPACEUTIL_H_

#include <string>
using std::string;

#include "DataTypes.h"
#include "Vector.h"

#include "SubspaceDefines.h"

///////////////////////////////////////

extern const double TOLERANCE; 

void tileToObjectCoords(Uint x, Uint y, Vector* p);
void objectToTileCoords(const Vector& p, Uint* x, Uint* y);
void objectToTileCoords(const Vector& p, int* x, int* y);

void setScreenCamera(const Vector& camera);
void objectToScreenCoords(const Vector& p, Vector* screen);
bool isOnScreen(const Vector& p);


static const int INVALID_PLAYER_ID = -1;
int getPlayerID(const SubspacePlayerMap& players, const string& name);

#endif