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
	DatabaseManager(); ///< default constructor
	~DatabaseManager(); ///< default destructor

	QSqlQuery returnOpenProcesses(); ///< return a qsqlquery with all pending processes

public slots:
	Q_INVOKABLE bool insertNewProcess(QString ip, QString path, QString direction); ///< insert a new process to send/receive a file
	Q_INVOKABLE QObject *retornaPendetesModel(); ///< returns all files that are been sending/receiving
	Q_INVOKABLE void updateDestDir(QString path); ///< Updates the destination dir of downloaded files from server
	Q_INVOKABLE QString getDestinationDir(); ///< returns the destinatioh dir
	Q_INVOKABLE void updateServerAddress(QString path); ///< Updates the server address to get files
	Q_INVOKABLE QString getServerAddress(); ///< return the server address to get files

private:
	QSqlDatabase db; ///< Database conection
	void verifyNewDatabase(); ///< Creates tables if the database is a new one
	bool alreadySending(QString path, QString ip); ///< verify if a file/IP are not in progress
	void updateSentReceive(QString ip, QString file); ///< update the number os sent/received pieces of a file
	QMLInterface model; ///< model to return data from DB
};
