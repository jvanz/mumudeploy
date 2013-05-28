#include "server/mumuserver.h"
#include <QString>

class NetworkManager
{

private:
	static NetworkInterface* mumuServer;

public:
	/*This function return a pointer to a mumuserver instance*/
	static NetworkInterface* getInstance( int port, QObject * parent)
	{
		if(!mumuServer){
			mumuServer = new MumuServer(port, parent);
		}
		return mumuServer;
	}
};
