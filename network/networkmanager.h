#include "server/mumuserver.h"
#include <QString>

class NetworkManager
{
public:
	/*This function return a pointer to a mumuserver instance*/
	static NetworkInterface* getInstance(QString filePath, QObject * parent)
	{
		return new MumuServer(filePath, parent);
	}

};
