
#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QStringList>
#include <qfile.h>

QT_BEGIN_NAMESPACE
class QTcpServer;
class QNetworkSession;
QT_END_NAMESPACE

class MumuServer : public QTcpServer
{
	Q_OBJECT

public:
	MumuServer(quint16 port, QObject* parent = 0) : QTcpServer(parent) 

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
