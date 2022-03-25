#ifndef _STATBOXPOINTSORT_H_
#define _STATBOXPOINTSORT_H_

#include <map>
using std::map;

#include "StatBoxPlayerSort.h"

class StatBoxPointSort :
	public StatBoxPlayerSort
{
public:
	virtual void init(const SubspacePlayerMap& players, const SubspacePlayer* myPlayer);

private:

	static void pointSort(SubspaceTextBox& box, PlayerList& pagePlayers, const SubspacePlayerMap& players, const SubspacePlayer* myPlayer);
};

#endif