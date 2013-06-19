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
	MumuServer();
	MumuServer(int,QObject * parent); 
	static MumuServer* instance;
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
	bool insertNewProcess(QString ip, QString path);
	static MumuServer* getInstance(int port, QObject * parent);

protected:
	void incomingConnection(int);

private slots:
	
	void clientConnecting();
	void showErrorMessage(QAbstractSocket::SocketError);
	void socketStateChanged(QAbstractSocket::SocketState);
};

#endif //__MUMUSERVER_H__
