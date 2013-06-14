#ifndef __MUMUSERVER_H__
#define __MUMUSERVER_H__

#include <QTcpServer>
#include <QFile>
#include <QList>
#include <QByteArray>

#include "mumuconnection.h"
#include "../networkinterface.h"
 
class MumuServer : public QTcpServer, public NetworkInterface
{
	Q_OBJECT

private:
	QList<MumuFile*> files;
	QStringList blackListFile; 
	QDir homeApp;
	QDataStream * in;
	QString filePath;
	QList<MumuConnection *> connections;
	DatabaseManager * databaseManager;
	int totalSplit;
	void openFiles();
	void openAndSplitFile();

public:
	void sendFile(QString);
	int getNumberOfParts();
	MumuServer(int,QObject * parent); 
	bool insertNewProcess(QString ip, QString path);

protected:
	void incomingConnection(int);

private slots:
	
	void clientConnecting();
	void showErrorMessage(QAbstractSocket::SocketError);
	void socketStateChanged(QAbstractSocket::SocketState);
};

#endif //__MUMUSERVER_H__
