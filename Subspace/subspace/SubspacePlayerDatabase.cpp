#include "SubspacePlayerDatabase.h"

SubspacePlayer* SubspacePlayerDatabase::getPlayer(const string& name)
{
	for(int i=0; i < this->size(); ++i)
	{
		SubspacePlayer* p = (*this)[i];
		if(p && p->getName() == name)//strcmp(p->getPlayerData()->info.name.c_str(), name.c_str()) == 0)
		{
			return p;
		}
		/*if(strcmp(p->info.name, name.c_str()) == 0)
		{
			return p;
		}*/
	}

	return 0;
}

const SubspacePlayer* SubspacePlayerDatabase::getPlayer(const string& name) const
{
	for(int i=0; i < this->size(); ++i)
	{
		const SubspacePlayer* p = this->getItem(i);
		if(p && p->getName() == name)//strcmp(p->getPlayerData()->info.name.c_str(), name.c_str()) == 0)
		{
			return p;
		}
	}

	return 0;
}

