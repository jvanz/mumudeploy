
#ifndef SERVER_H
#define SERVER_H

#include <QThread>
#include <QStringList>
#include <qfile.h>

QT_BEGIN_NAMESPACE
class QTcpServer;
class QNetworkSession;
QT_END_NAMESPACE

class Server : public QThread
{
	Q_OBJECT

public:
	Server();

protected:
	void run();

private slots:
	void sessionOpened();
	void sendFortune();

private:
	QTcpServer *tcpServer;
	QStringList fortunes;
	QNetworkSession *networkSession;
};

#endif
