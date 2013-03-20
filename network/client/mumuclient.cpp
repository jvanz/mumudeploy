
#include "mumuclient.h"
#include <iostream>
#include <QHostAddress>

MumuClient::MumuClient(QObject *parent) : tcpSocket(parent)
{
	connectMumuServer();
}

/**
 * This function conects the MumuClient to the MumuServer
 */
bool MumuClient::connectMumuServer()
{
	connect(&tcpSocket,SIGNAL(connected()),this,SLOT(serverConnected()));
	tcpSocket.connectToHost(QHostAddress::LocalHost, 6666);
}

void MumuClient::serverConnected()
{
	std::cout<<"Client connected!"<<std::endl;
	connected = true;
}
