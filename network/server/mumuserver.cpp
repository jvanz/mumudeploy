
#include "mumuserver.h"
#include <iostream>
#include <QDir>
#include <QTcpSocket>

/** Constructor */
MumuServer::MumuServer(QObject *parent) : QTcpServer(parent) 
{
	if(this->listen(QHostAddress::Any, 6666)){
		std::cout<<"The MumuServer is listening any ip address on port 666."<<std::endl;
	}
	if(this->isListening()){
		std::cout<<"The server is listening"<<std::endl;
	}
	connect(this, SIGNAL(newConnection()),this,SLOT(clientConnecting()));

	openFile();
	std::cout<<"Constructor done!."<<std::endl;
}

void MumuServer::clientConnecting()
{
	std::cout<<"Client wants connect!"<<std::endl;
}

void MumuServer::openFile()
{
	QString pathFile = QDir::homePath() + "/server/file";
	file = new QFile(QDir::toNativeSeparators(pathFile));
	if(file->exists()){
		std::cout<<"File opened!"<<std::endl;
	}
}

void MumuServer::incomingConnection(int socketDescription)
{
	std::cout<<"Incoming connection... socket description = " + socketDescription<<std::endl;
	QTcpSocket tcpSocket;
	tcpSocket.setSocketDescriptor(socketDescription);
	tcpSocket.write("teste");
	tcpSocket.disconnectFromHost();
	tcpSocket.waitForDisconnected();
}
