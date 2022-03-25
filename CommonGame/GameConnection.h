//David Millman

#ifndef _GAMECONNECTION_H_
#define _GAMECONNECTION_H_

#include <string>

using std::string;

#include "DataTypes.h"
#include "Socket.h"
#include "FileLogger.h"
#include "Thread.h"
#include "Serializable.h"

class GameConnection :
	public FileLogger
{
public:

	typedef int PacketHandlerFunc;

	GameConnection();
	virtual ~GameConnection();

	virtual bool connect(const string& host, Uint16 port);
	virtual bool disconnect();

	//virtual bool sendObject(Serializable& object);
	//virtual bool receiveObject(Serializable* object);

protected:

	//bool sendString(const string& s);

	Socket socket;
};

#endif