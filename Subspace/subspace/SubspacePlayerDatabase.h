// David Millman

#ifndef _SUBSPACEPLAYERDATABASE_H_
#define _SUBSPACEPLAYERDATABASE_H_

#include "Database.h"

#include "SubspacePlayer.h"

class SubspacePlayerDatabase :
	public Database<SubspacePlayer>
{
public:

	SubspacePlayer* getPlayer(const string& name);
	const SubspacePlayer* getPlayer(const string& name) const;


private:
	
};

#endif