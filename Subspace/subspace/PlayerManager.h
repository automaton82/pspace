//David Millman

#include <map>

#include "DataTypes.h"
#include "SSPlayer.h"

#ifndef _PlayerManager_
#define _PlayerManager_

typedef Uint PlayerID;

typedef std::map<Uint, SSPlayer> PlayerMap;

class PlayerManager
{
public:
	PlayerManager();
	
	PlayerID addPlayer(SSPlayer& p);
	void removePlayer(PlayerID id);

	SSPlayer* getPlayer(PlayerID id);
	SSPlayer* getPlayer(string name);

private:

	PlayerMap players_;
};

#endif
