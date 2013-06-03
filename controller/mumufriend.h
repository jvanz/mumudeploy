#ifndef __MUMUFRIEND_H__
#define __MUMUFIREND_H__

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

public slots:
	Q_INVOKABLE bool insertNewProcess(QString ip, QString path, int parts); ///< Send a file by the server
	Q_INVOKABLE QObject *retornaPendentesModel(); ///< wrapper of method from DatabaseManager
	Q_INVOKABLE QString getDestinationDir(); ///< wrapper of method from DatabaseManager
	Q_INVOKABLE QString getServerAddress(); ///< wrapper of method from DatabaseManager
	Q_INVOKABLE void updateDestDir(QString path); ///< wrapper of method from DatabaseManager
	Q_INVOKABLE void updateServerAddress(QString address); ///< wrapper of method from DatabaseManager

public:
	MumuFriend(QObject *parent = 0); ///< Default constructor
	QSqlQuery returnOpenProcess(); ///< Return all pending actions that needs to be handled by server
	void updateSendPieces(QString ip, QString file); ///< Updates the number of packets sent by server

private:
	NetworkInterface *server; ///< Instance of the server
};
#endif //__MUMUFRIEND_H__
