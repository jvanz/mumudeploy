
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
	QList<QFile*> files;
	QStringList blackListFile; 
	QDir homeApp;
	QDataStream * in;
	QString filePath;
	QList<MumuConnection *> connections;
	int totalSplit;
	void openFiles();
	void openAndSplitFile();
	QList<QByteArray> splitFile(QFile*, int);

public:
	void sendFile(QString);
	MumuServer(QDir,QObject * parent); 

protected:
	void incomingConnection(int);

private slots:
	
	void clientConnecting();
	void showErrorMessage(QAbstractSocket::SocketError);
	void socketStateChanged(QAbstractSocket::SocketState);
};

