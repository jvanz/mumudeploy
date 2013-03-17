
#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QFile>
#include <QObject>

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

class Client : public QObject
{
	Q_OBJECT

public:
	Client();
	void run();

private slots:
	void requestNewFortune();
	void readFortune();
	void displayError(QAbstractSocket::SocketError socketError);
	void sessionOpened();

private:
	QTcpSocket *tcpSocket;
	QString currentFortune;
	quint16 blockSize;

	QNetworkSession *networkSession;
};

#endif
