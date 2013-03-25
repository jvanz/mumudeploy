#include <iostream>
#include <QDir>

#include "mumuconnection.h"


MumuConnection::MumuConnection(int socketDescriptor, QObject * parent) : QTcpSocket(parent)
{
	if(this->setSocketDescriptor(socketDescriptor)){
		std::cout << "Socket descriptor setted" << std::endl;
	}

	connect(this,SIGNAL(connected()),this,SLOT(socketConnected()));	
	connect(this,SIGNAL(disconnected()),this,SLOT(socketDisconnected()));	
	connect(this,SIGNAL(hostFound()),this,SLOT(socketHostFound()));	
	connect(this,SIGNAL(readyRead()),this,SLOT(processData()));
	statusConnection = -1;
}

void MumuConnection::clientReady()
{
	std::cout << "Sending file to client" << std::endl;
	sendMsgToClient("FILE");
}

void MumuConnection::sendMsgToClient(QString msg)
{
	std::cout << "Sending to client: " << msg.toStdString() << std::endl;
	int bytesWriten = write(msg.toUtf8());
	std::cout << "Bytes writen = " << bytesWriten << std::endl;
	
}

int MumuConnection::getState()
{
	return this->state();
}

void MumuConnection::connectedSocketSignals()
{
}

void MumuConnection::socketConnected()
{
	std::cout << "Socket " << id.toStdString() << " connected." << std::endl; 
}

void MumuConnection::socketDisconnected()
{
	std::cout << "Socket " << id.toStdString() << " disconnected." << std::endl; 
}

void MumuConnection::socketError(QAbstractSocket::SocketError error)
{
	std::cout << "Socket " << id.toStdString() << " ERROR! Code = " << error << std::endl; 
}

void MumuConnection::socketHostFound()
{
	std::cout << "Socket " << id.toStdString() << " host found." << std::endl; 
}

void MumuConnection::socketStateChanged(QAbstractSocket::SocketState state)
{
	std::cout << "Socket " << id.toStdString() << " State = " << state << std::endl; 
}

void MumuConnection::setId(QString id)
{
	this->id = id;
}

QString MumuConnection::getId()
{
	return this->id;
}

void MumuConnection::sendFile()
{
	if(file){
		buffer = file->readAll();
		std::cout << "Socket " << id.toStdString() << " sending file. Bytes = " << buffer.size() << std::endl; 
		write(buffer);
	}
}

void MumuConnection::openFile()
{
	QString pathFile = QDir::homePath() + "/server/1-04 Stairway To Heaven.m4a";
	file = new QFile(QDir::toNativeSeparators(pathFile));
	if(file->exists()){
		std::cout<<"File opened!"<<std::endl;
		std::cout<<pathFile.toStdString()<<std::endl;
	}
	file->open(QIODevice::ReadOnly);
	
}

void MumuConnection::processData()
{
	std::cout << "Socket " << id.toStdString() << " has data to process. " << std::endl; 
	buffer = readAll();
	QString msg(buffer);
	std::cout << "Message: " << msg.toStdString() << std::endl; 
	if(msg == "GREETING"){
		this->clientReady();
		statusConnection = 1;
	}else if(msg == "OK" & statusConnection == 1){
		this->openFile();
		this->sendFile();
	}
}
