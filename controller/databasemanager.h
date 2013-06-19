#ifndef __DATABASEMANAGER_H__
#define __DATABASEMANAGER_H__
#include <QtDeclarative>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "qmlinterface.h"

/**
 * DatabaseManager - Manager all DB interactions
 */
class DatabaseManager : public QObject
{
	Q_OBJECT
public:
	static DatabaseManager *getInstance();
	QSqlQuery returnOpenProcesses(); ///< return a qsqlquery with all pending processes

public slots:
	bool insertNewProcess(QString ip, QString path, QString direction, int parts); ///< insert a new process to send/receive a file
	QObject *retornaPendetesModel(); ///< returns all files that are been sending/receiving
	void updateDestDir(QString path); ///< Updates the destination dir of downloaded files from server
	QString getDestinationDir(); ///< returns the destinatioh dir
	void updateServerAddress(QString path); ///< Updates the server address to get files
	QString getServerAddress(); ///< return the server address to get files
	void updateSentReceive(QString ip, QString file); ///< update the number os sent/received pieces of a file
	int nextPiece(QString file, QString ip); ///< brings the next piece that needs to be send
	void updateSentFlag(QString ip, QString path); ///< the file was sucessfully sent

private:
	DatabaseManager(); ///< default constructor
	~DatabaseManager(); ///< default destructor

	QSqlDatabase db; ///< Database conection
	void verifyNewDatabase(); ///< Creates tables if the database is a new one
	bool alreadySending(QString path, QString ip); ///< verify if a file/IP are not in progress
	QMLInterface model; ///< model to return data from DB

	static DatabaseManager *instance;
};

#endif
