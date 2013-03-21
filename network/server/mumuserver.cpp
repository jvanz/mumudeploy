
#include "mumuserver.h"
#include <iostream>
#include <QDir>
#include <QTcpSocket>

/** Constructor */
MumuServer::MumuServer(QObject *parent) : QTcpServer(parent) 
{
	if(this->listen(QHostAddress::Any, 8080)){
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
	QTcpSocket tcpSocket;
	connect(&tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(showErrorMessage(QAbstractSocket::SocketError)));
	connect(&tcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(socketStateChanged(QAbstractSocket::SocketState)));
	if(tcpSocket.setSocketDescriptor(socketDescription)){
		if(tcpSocket.isOpen()){
			if(tcpSocket.isWritable()){
				std::cout<<"Socket is writable!"<<std::endl;
			}
			int writenBytes = tcpSocket.write(file->readAll());
			std::cout<<"Bytes writen = "<<writenBytes<<std::endl;
			tcpSocket.flush();
			tcpSocket.disconnectFromHost();
			tcpSocket.waitForDisconnected();
		}
	}
}

void MumuServer::showErrorMessage(QAbstractSocket::SocketError socketError)
{
	std::cout<<"ERROR: "<<socketError<<std::endl;
}

void MumuServer::socketStateChanged(QAbstractSocket::SocketState state)
{
	std::cout<<"Socket state = "<<state<<std::endl;
}
