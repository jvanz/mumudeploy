
#include <QTcpSocket>
#include <QDataStream>
#include <QTextStream>
#include <QObject>
#include <QFile>
#include <QByteArray>
#include <QHostAddress>

#include "../commum/mumufile.h"

class MumuClient : QObject
{
	Q_OBJECT

public:
	MumuClient(QString,QHostAddress,int,QObject *parent);
private:
	bool connected;
	QTcpSocket tcpSocket;
	qint64 nextBlockSize;
	QFile * file;
	QDataStream * inFile;
	QByteArray buffer;
	QString filePath;
	QHostAddress ipServer;
	int portServer;
	int statusConnection; // 1 = greeting sent ; 2 = file request ; 3 = file recive
	char * bytes;

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

