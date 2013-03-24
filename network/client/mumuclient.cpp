
#include "mumuclient.h"
#include <iostream>
#include <QHostAddress>
#include <QDir>

MumuClient::MumuClient(QObject *parent) : tcpSocket(parent)
{
	connect(&tcpSocket,SIGNAL(readyRead()),this,SLOT(readFile()));
	connect(&tcpSocket,SIGNAL(disconnected()),this,SLOT(closeStream()));
	connect(&tcpSocket,SIGNAL(connected()),this,SLOT(serverConnected()));
	connect(&tcpSocket,SIGNAL(hostFound()),this,SLOT(hostServerFound()));
	connect(&tcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(socketStateChanged(QAbstractSocket::SocketState)));
	connectMumuServer();
}

/**
 * This function conects the MumuClient to the MumuServer
 */
bool MumuClient::connectMumuServer()
{
	tcpSocket.connectToHost(QHostAddress::LocalHost, 8080);
	std::cout<<"TCP Socket state? "<<tcpSocket.state()<<std::endl;
	return tcpSocket.isOpen();
}

void MumuClient::serverConnected()
{
	std::cout<<"Client connected!"<<std::endl;
	connected = true;
}

void MumuClient::readFile()
{
	std::cout<<"Receiving data"<<std::endl;
	in = new QDataStream(&tcpSocket);
	in->setVersion(QDataStream::Qt_4_0);
	std::cout<<"Bytes Available = "<<tcpSocket.bytesAvailable()<<std::endl;
	std::cout<<"Bytes to write = "<<tcpSocket.bytesToWrite()<<std::endl;
	if(openFile()){
		std::cout<<"Writing on the file"<<std::endl;
		out = new QDataStream(file);
		char * readInBytes;
		in->readRawData(readInBytes, tcpSocket.bytesToWrite());
		out->writeRawData(readInBytes,tcpSocket.bytesToWrite());
		file->flush();
	}else{
		std::cout<<"Problem in the file"<<std::endl;
	}
}

bool MumuClient::openFile()
{
	if(file){
		return true;
	}
	QString pathFile = QDir::homePath() + "/client/1-04 Stairway To Heaven.m4a";
	std::cout<<pathFile.toStdString()<<std::endl;
	file = new QFile(QDir::toNativeSeparators(pathFile));
	std::cout<<file<<std::endl;
	if(file->exists()){ // File exists
		std::cout<<"File opened!"<<std::endl;
	}	
	return file->open(QIODevice::WriteOnly);
}

void MumuClient::closeStream()
{
	std::cout<<"Closing stream"<<std::endl;
	if(file){
		file->flush();
		file->close();
	}
}

void MumuClient::hostServerFound()
{
	std::cout<<"Host Server!"<<std::endl;
}

void MumuClient::socketStateChanged(QAbstractSocket::SocketState state)
{
	std::cout<<"Socket state = "<<state<<std::endl;
}
