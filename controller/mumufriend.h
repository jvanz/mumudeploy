#ifndef __MUMUFRIEND_H__

#include <QObject>
#include <QSqlQuery>
#include "networkmanager.h"

class MumuFriend : public QObject
{
	Q_OBJECT

private:
	NetworkInterface * server;

public:
	MumuFriend(QObject *parent = 0);
	Q_INVOKABLE void sendFile(QString path);
	QSqlQuery returnOpenProcess();
};
#endif //__MUMUFRIEND_H__
