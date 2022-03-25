#ifndef _STATBOXPLAYERSORTPOINTS_H_
#define _STATBOXPLAYERSORTPOINTS_H_

#include "StatBoxPlayerSort.h"

class StatBoxPlayerSortPoints :
	public StatBoxPlayerSort
{
public:
	virtual void init(const SubspacePlayerMap& players, const SubspacePlayer* myPlayer);
};

#endif