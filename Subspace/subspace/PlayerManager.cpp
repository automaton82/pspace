//David Millman

#include "PlayerManager.h"

PlayerManager::PlayerManager()
{
}

PlayerID PlayerManager::addPlayer(SSPlayer& p)
{
	players_[p.data()->info_.playerID] = p;
	return (PlayerID)p.data()->info_.playerID;
}

void PlayerManager::removePlayer(PlayerID id)
{
	players_.erase(id);
}

SSPlayer* PlayerManager::getPlayer(PlayerID id)
{
	if(players_.find(id) != players_.end())
		return &players_[id];
	else
		return 0;
}

SSPlayer* PlayerManager::getPlayer(string name)
{
	PlayerMap::iterator i;
	for(i = players_.begin(); i != players_.end(); i++)
	{		
		if(i->second.data()->info_.name == name)
			return &(i->second);
	}
	return 0;
}

