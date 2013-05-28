#include "server/mumuserver.h"
#include <QString>

class NetworkManager
{

public:
	NetworkInterface* server;

	/*This function return a pointer to a mumuserver instance*/
	NetworkInterface* getInstance( int port, QObject * parent)
	{
		if(!server){
			server = new MumuServer(port, parent);
		}
		return server;
	}
};
