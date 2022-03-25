#ifndef _SUBSPACEDEFINES_H_
#define _SUBSPACEDEFINES_H_

#include <map>
#include <vector>

#include "DataTypes.h"

//////////////////////////

class SubspacePlayer;

typedef std::vector<SubspacePlayer*> SubspacePlayerList;
typedef std::map<Uint, SubspacePlayer*> SubspacePlayerMap;

#endif