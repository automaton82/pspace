#ifndef _STATBOXTEAMSTATS_H_
#define _STATBOXTEAMSTATS_H_

#include <map>
using std::map;

#include "StatBoxPlayerSort.h"

class StatBoxTeamStats :
	public StatBoxPlayerSort
{
public:
	virtual void init(const SubspacePlayerMap& players, const SubspacePlayer* myPlayer);

	// accessors
	virtual SubspacePlayer* getSelectedPlayer() const;

	// mutators
	virtual void selectPlayer(SubspacePlayer* player);
private:

	void teamStatistics(SubspaceTextBox& box, PlayerList& players, const SubspacePlayerMap& playerMap, const SubspacePlayer* myPlayer);
};

#endif