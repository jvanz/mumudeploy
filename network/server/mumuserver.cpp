
#include "mumuserver.h"
#include <iostream>
#include <QDir>
#include <QTcpSocket>

/** Constructor */
MumuServer::MumuServer(QObject *parent) : QTcpServer(parent) 
{
	if(this->listen(QHostAddress::Any, 8080)){
		std::cout<<"The MumuServer is listening any ip address on port " << this->serverPort() << std::endl;
	}
	if(this->isListening()){
		std::cout<<"The server is listening"<<std::endl;
	}
	connect(this, SIGNAL(newConnection()),this,SLOT(clientConnecting()));

//	openFile();
	std::cout<<"Constructor done!."<<std::endl;
}

void MumuServer::clientConnecting()
{
	std::cout<<"Client wants connect!"<<std::endl;
	std::cout << "Total clients connected = " << connections.size() << std::endl;
}

void MumuServer::openFile()
{
	QString pathFile = QDir::homePath() + "/server/1-04 Stairway To Heaven.m4a";
	file = new QFile(QDir::toNativeSeparators(pathFile));
	if(file->exists()){
		std::cout<<"File opened!"<<std::endl;
		std::cout<<pathFile.toStdString()<<std::endl;
	}
	file->open(QIODevice::ReadOnly);
	
}

void MumuServer::incomingConnection(int socketDescription)
{
	std::cout<<"Incoming connection..."<<std::endl;
	MumuConnection * connection = new MumuConnection(socketDescription,this);
	std::cout<<"connection created"<<std::endl;
	connection->setId("TESTE");
	connections.append(connection); 
	std::cout<<"Leaving incoming connection..."<<std::endl;
}

void MumuServer::showErrorMessage(QAbstractSocket::SocketError socketError)
{
	std::cout<<"ERROR: "<<socketError<<std::endl;
}

void MumuServer::socketStateChanged(QAbstractSocket::SocketState state)
{
	std::cout<<"Socket state = "<<state<<std::endl;
}