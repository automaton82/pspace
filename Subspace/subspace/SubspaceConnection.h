//David Millman

#ifndef _SUBSPACECONNECTION_H_
#define _SUBSPACECONNECTION_H_

class SubspaceZone;
#include "SubspaceZone.h"

class SubspaceProtocol;
#include "SubspaceProtocol.h"

class SubspaceConnection
{
public:

	SubspaceConnection();
	~SubspaceConnection();

	/*************************
	 * Layered functionality *
	 *************************/

	bool connect(const string& host, Uint16 port, Uint timeout = 2000);
	bool disconnect();

	void setZone(SubspaceZone* zone);
	
	void setHeader(const string& header);
	void setLog(FILE* file);

private:

	SubspaceProtocol mainProtocol_;
};

#endif