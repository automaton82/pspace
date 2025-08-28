//David Millman

#include <map>

#include "DataTypes.h"
#include "SubspacePlayer.h"

#ifndef _PlayerManager_
#define _PlayerManager_

typedef Uint PlayerID;

typedef std::map<Uint, SubspacePlayer> PlayerMap;

class PlayerManager
{
public:
	PlayerManager();
	
	PlayerID addPlayer(SubspacePlayer& p);
	void removePlayer(PlayerID id);

	SubspacePlayer* getPlayer(PlayerID id);
	SubspacePlayer* getPlayer(string name);

private:

	PlayerMap players_;
};

#endif
