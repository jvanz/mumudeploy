
#ifndef __NETWORKMANAGER_H__
#define __NETWORKMANAGER_H__
#include "server/mumuserver.h"
#include <QString>

class NetworkManager
{

public:
	/*This function return a pointer to a mumuserver instance*/
	NetworkInterface * getInstance( int port, QObject * parent)
	{
		return MumuServer::getInstance(port, parent);
	}
};
#endif
