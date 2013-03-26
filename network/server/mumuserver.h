
#include <QTcpServer>
#include <QFile>
#include <QList>

#include "mumuconnection.h"

class MumuServer : public QTcpServer
{
	Q_OBJECT

private:
	QFile * file;
	QDataStream * in;
	QList<MumuConnection *> connections;
	void openFile();

public:
	MumuServer(QObject * parent); 

protected:
	void incomingConnection(int);

private slots:
	
	void clientConnecting();
	void showErrorMessage(QAbstractSocket::SocketError);
	void socketStateChanged(QAbstractSocket::SocketState);
};

