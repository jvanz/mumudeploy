
#include "mumuclient.h"
#include <iostream>
#include <QHostAddress>

MumuClient::MumuClient(QObject *parent) : tcpSocket(parent)
{
	connectMumuServer();
	std::cout<<"passou pelo construtor"<<std::endl;
}

/**
 * This function conects the MumuClient to the MumuServer
 */
bool MumuClient::connectMumuServer()
{
	connect(&tcpSocket,SIGNAL(connected()),this,SLOT(serverConnected()));
	QHostAddress serverIp("127.0.0.1");
	tcpSocket.connectToHost(QHostAddress::LocalHost, 6666);
	std::cout<<"passou pelo connect"<<std::endl;
}

void MumuClient::serverConnected()
{
	std::cout<<"Client connected!"<<std::endl;
	connected = true;
}
