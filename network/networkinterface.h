#ifndef __NETWORKINTERFACE_H__
#define __NETWORKINTERFACE_H__

#include <QString>

class NetworkInterface
{
public:
	virtual void sendFile(QString) = 0;
	virtual int getNumberOfParts() = 0;
};

#endif //__NETWORKINTERFACE_H__
