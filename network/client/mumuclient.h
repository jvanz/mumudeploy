
#include <QTcpSocket>
#include <QDataStream>
#include <QObject>
#include <QFile>

class MumuClient : QObject
{
	Q_OBJECT

public:
	MumuClient(QObject *parent);
private:
	bool connected;
	QTcpSocket tcpSocket;
	QDataStream * in;
	QDataStream * out;
	QFile * file;

	bool connectMumuServer();
	bool openFile();

private slots:
	void serverConnected();
	void readFile();
	void closeStream();
	void hostServerFound();
	void socketStateChanged(QAbstractSocket::SocketState);
};

