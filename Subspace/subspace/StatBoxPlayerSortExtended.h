#ifndef _STATBOXPLAYERSORTEXTENDED_H_
#define _STATBOXPLAYERSORTEXTENDED_H_

#include "StatBoxPlayerSort.h"

class StatBoxPlayerSortExtended :
	public StatBoxPlayerSort
{
public:
	virtual void init(const SubspacePlayerMap& players, const SubspacePlayer* myPlayer);

private:

	static string getStatsString(const SubspacePlayer* p, bool newline);

	static void playerSortExtended(SubspaceTextBox& page, PlayerList& pagePlayers, const SubspacePlayerMap& players, const SubspacePlayer* myPlayer);
};

#endif