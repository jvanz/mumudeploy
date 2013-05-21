#include <iostream>
#include <QDir>

#include "mumuconnection.h"


MumuConnection::MumuConnection(int socketDescriptor,QList<MumuFile*>* fileList, QObject * parent) : files(fileList), QTcpSocket(parent)
{
	if(this->setSocketDescriptor(socketDescriptor)){
		Util::logMessage("Socket descriptor setted");
	}

	connect(this,SIGNAL(connected()),this,SLOT(socketConnected()));	
	connect(this,SIGNAL(disconnected()),this,SLOT(socketDisconnected()));	
	connect(this,SIGNAL(hostFound()),this,SLOT(socketHostFound()));	
	connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)));
	connect(this,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(socketStateChanged(QAbstractSocket::SocketState)));
	connect(this,SIGNAL(readyRead()),this,SLOT(processData()));
	statusConnection = -1;
	nextBlockSize = 0;
}

void MumuConnection::clientReady()
{
	Util::logMessage("Sending file to client");
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

/**
 * This funtion send the file for client. When we write de file on the socket is necessary use writeRawData because this is the only
 * way to write just the bytes on the socket. Otherwise, the QT will serialize the whole object.
 */ 
void MumuConnection::sendFile()
{
	if(this->files->size() > 0){
		MumuFile * file = this->files->at(0);
		file->getFile()->open(QIODevice::ReadOnly);
		QByteArray blockFile = MumuFile::compress(file->getFile()->readAll());
        	write(blockFile.constData(),blockFile.size());
		file->getFile()->close();
		this->disconnectFromHost();
		Util::logMessage("File sent");
		return;
	}
	Util::logMessage("File did not send");
}

void MumuConnection::openFile()
{
	file = new QFile(QDir::toNativeSeparators(filePath));
	if(file->exists()){
		std::cout<<"File opened!"<<std::endl;
		std::cout<<filePath.toStdString()<<std::endl;
	}
	file->open(QIODevice::ReadOnly);
	
}

void MumuConnection::processData()
{
	QByteArray block = Util::processData(this);
	this->processBlock(block);
}

void MumuConnection::processBlock(QByteArray block)
{
 	Util::logMessage("Bloco para processar");
 	Util::logMessage("Tamanho = " + QString::number(block.size()));
	
	QDataStream in(&block, QIODevice::ReadOnly);
	char * data = block.data();
	Util::logMessage("Data size = " + QString::number(sizeof(*data)/sizeof(char)));
	printf("data = %d\n", *data);
	if(*data == SOH){
		Util::logMessage("SOH");
	}
}

void MumuConnection::sendBytesToClient(char * data)
{
	Util::sendBytesTo(data,this);
}


void MumuConnection::sendMsgToClient(char msg)
{
	Util::sendMsgTo(msg,this);	
}

void MumuConnection::sendAckToClient()
{
	this->sendMsgToClient(ACK);
}

void MumuConnection::sendNakToClient()
{
	this->sendMsgToClient(NAK);
}
