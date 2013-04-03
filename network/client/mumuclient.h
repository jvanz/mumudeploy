
#include <QTcpSocket>
#include <QDataStream>
#include <QTextStream>
#include <QObject>
#include <QFile>
#include <QByteArray>

class MumuClient : QObject
{
	Q_OBJECT

public:
	MumuClient(QObject *parent);
private:
	bool connected;
	QTcpSocket tcpSocket;
	quint16 nextBlockSize;
	QFile * file;
	QDataStream * inFile;
	QByteArray buffer;
	int statusConnection;

	bool connectMumuServer();
	bool openFile();
	void sendGreeting();
	void sendOk();
	void sendMsgToServer(QString);

private slots:
	void serverConnected();
	void readFile();
	void closeStream();
	void hostServerFound();
	void socketStateChanged(QAbstractSocket::SocketState);
	void catchError(QAbstractSocket::SocketError);
	void proxyAuthentication(const QNetworkProxy & proxy, QAuthenticator * authenticator);
};
