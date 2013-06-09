#ifndef __MUMUCONNECTION_H__
#define __MUMUCONNECTION_H__

#include <QtNetwork>
#include <QString>
#include <QByteArray>
#include <QFile>

#include "../commum/mumufile.h"
#include "../../controller/databasemanager.h"

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
	MumuFile * currentFile;
	QByteArray * currentBlock;
	int statusConnection; // 1 = SOH recive
	qint64 nextBlockSize;
	QList<MumuFile*>* files;
	DatabaseManager * dbManager;
	QHostAddress clientIP; /// Client's IP

	void sendMsgToClient(quint16);
	void sendAckToClient();
	void sendNakToClient();
	void clientReady();
	bool sendFile();
	void processBlock(QByteArray);
	void sendBytesToClient(QByteArray);
	bool sendFileDescriptor();
	bool registreIP(QHostAddress);
	void sendBlockToClient(QByteArray block);

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
