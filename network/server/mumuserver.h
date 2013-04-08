
#include <QTcpServer>
#include <QFile>
#include <QList>
#include <QByteArray>

#include "mumuconnection.h"

class MumuServer : public QTcpServer
{
	Q_OBJECT

private:
	QList<QFile*> files;
	QStringList blackListFile; 
	QDir homeApp;
	QDataStream * in;
	QList<MumuConnection *> connections;
	int totalSplit;
	void openFiles();
	void openAndSplitFile();
	QList<QByteArray> splitFile(QFile*, int);

public:
	MumuServer(QDir,QObject * parent); 

protected:
	void incomingConnection(int);

private slots:
	
	void clientConnecting();
	void showErrorMessage(QAbstractSocket::SocketError);
	void socketStateChanged(QAbstractSocket::SocketState);
};

