//David Millman

#include "PlayerManager.h"

PlayerManager::PlayerManager()
{
}

PlayerID PlayerManager::addPlayer(SubspacePlayer& p)
{
	players_[p.getId()] = p;
	return (PlayerID)p.getId();
}

void PlayerManager::removePlayer(PlayerID id)
{
	players_.erase(id);
}

SubspacePlayer* PlayerManager::getPlayer(PlayerID id)
{
	if(players_.find(id) != players_.end())
		return &players_[id];
	else
		return 0;
}

SubspacePlayer* PlayerManager::getPlayer(string name)
{
	PlayerMap::iterator i;
	for(i = players_.begin(); i != players_.end(); i++)
	{		
		if(i->second.getName() == name)
			return &(i->second);
	}
	return 0;
}

