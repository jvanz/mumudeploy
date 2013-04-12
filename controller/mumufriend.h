#ifndef __MUMUFRIEND_H__

#include <QObject>
#include "server/networkmanager.h"
#include "server/networinterface.h"

class MumuFriend : public QObject
{
	Q_OBJECT

private:
	NetworkInterface * server;

public:
	MumuFriend(QObject *parent = 0);
	Q_INVOKABLE void sendFile(QString path);
};
#endif //__MUMUFRIEND_H__
