#ifndef __MUMUCONNECTION_H__
#define __MUMUCONNECTION_H__

#include <QtNetwork>
#include <QString>
#include <QByteArray>
#include <QFile>

#include "../commum/mumufile.h"

class MumuConnection : QTcpSocket
{
	Q_OBJECT


enum mumuConnectionState{
	NotGreeting,
	Ready
};

private:
	QString id;
	QString filePath;
	QByteArray buffer;
	QFile * file;
	int statusConnection;
	quint16 nextBlockSize;
	QList<MumuFile*>* files;

	void sendMsgToClient(QString);
	void clientReady();
	void sendFile();
	void openFile();

public:
	MumuConnection(int,QList<MumuFile*>*,QObject * parent = 0);

	void setId(QString);
	QString getId();
	void connectedSocketSignals();
	int getState();

public slots:
	
	void socketConnected();
	void socketDisconnected();
	void socketError(QAbstractSocket::SocketError);
	void socketHostFound();
	void socketStateChanged(QAbstractSocket::SocketState);
	void processData();
};

#endif //__MUMUCONNECTION_H__
