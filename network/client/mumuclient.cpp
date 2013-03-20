
#include "mumuclient.h"
#include <iostream>
#include <QHostAddress>
#include <QDir>

MumuClient::MumuClient(QObject *parent) : tcpSocket(parent)
{
	connectMumuServer();
	connect(&tcpSocket,SIGNAL(readyRead()),this,SLOT(readFile()));
	connect(&tcpSocket,SIGNAL(disconnected()),this,SLOT(closeStream()));
	
}

/**
 * This function conects the MumuClient to the MumuServer
 */
bool MumuClient::connectMumuServer()
{
	connect(&tcpSocket,SIGNAL(connected()),this,SLOT(serverConnected()));
	tcpSocket.connectToHost(QHostAddress::LocalHost, 6666);
	return true;
}

void MumuClient::serverConnected()
{
	std::cout<<"Client connected!"<<std::endl;
	connected = true;
}

void MumuClient::readFile()
{
	in = new QDataStream(&tcpSocket);
	in->setVersion(QDataStream::Qt_4_0);
	if(openFile()){
		out = new QDataStream(file);	
	}
	(*out)<<(*in);
}

bool MumuClient::openFile()
{
	QString pathFile = QDir::homePath() + "/client/clientfile";
	file = new QFile(QDir::toNativeSeparators(pathFile));
	if(file->exists()){ // File exists
		std::cout<<"File opened!"<<std::endl;
	}	
	return file->open(QIODevice::WriteOnly);
}

void MumuClient::closeStream()
{
	file->flush();
	file->close();
}
