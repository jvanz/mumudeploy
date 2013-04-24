#ifndef __MUMUFRIEND_H__

#include <QObject>
#include <QSqlQuery>
#include "networkmanager.h"

/**
 * MumuFriend - A class that is friend of everyone
 * This class is a glue between the system.
 */

class MumuFriend : public QObject
{
	Q_OBJECT

private:
	NetworkInterface *server; ///< Instance of the server

public:
	MumuFriend(QObject *parent = 0); ///< Default constructor
	Q_INVOKABLE bool sendFile(QString ip, QString path); ///< Send a file by the server
	QSqlQuery returnOpenProcess(); ///< Return all pending actions that needs to be handled by server
	void updateSendPieces(QString ip, QString file); ///< Updates the number of packets sent by server
};
#endif //__MUMUFRIEND_H__
