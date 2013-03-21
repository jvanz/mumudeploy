
#include <QTcpServer>
#include <QFile>

class MumuServer : public QTcpServer
{
	Q_OBJECT

private:
	QFile * file;
	QDataStream * in;

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

