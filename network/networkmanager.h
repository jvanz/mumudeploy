#include "./mumuserver.h"
#include <QString>

class NetworkManager
{
public:
	/*This function return a pointer to a mumuserver instance*/
	static NetworkInterface* getInstance(QObject* parent, QString filePath)
	{
		return new MumuServer(filePath, parent);
	}

};
