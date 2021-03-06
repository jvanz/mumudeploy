
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
	MumuClient(QHostAddress,int,QObject *parent);
private:
	bool connected;
	QTcpSocket tcpSocket;
	qint64 nextBlockSize;
	MumuFile * file;
	MumuFile * currentFile;
	QDataStream * inFile;
	QByteArray buffer;
	QHostAddress ipServer;
	int portServer;
	int statusConnection; // 1 = greeting sent ; 2 = file request ; 3 = file recive
	char * bytes;
	quint8 currentBlock;

	bool connectMumuServer();
	void sendGreeting();
	void sendOk();
	void sendMsgToServer(quint16);
	void sendAckToServer();
	void sendNakToServer();
	void requestFilesToServer();
	void processBlock(QByteArray);
	bool processFileDescriptorBlock(QByteArray);
	void sendBytesToServer(QByteArray);
	bool processFileBlock(QByteArray);

private slots:
	void serverConnected();
	void readFile();
	void closeStream();
	void hostServerFound();
	void socketStateChanged(QAbstractSocket::SocketState);
	void catchError(QAbstractSocket::SocketError);
	void proxyAuthentication(const QNetworkProxy & proxy, QAuthenticator * authenticator);
};

