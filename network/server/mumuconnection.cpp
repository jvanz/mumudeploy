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
	this->nextBlockSize = 0;
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
bool MumuConnection::sendFile()
{
	if(this->files->size() > 0){
		MumuFile * file = this->files->at(0);
		QByteArray block = Util::getBlockFile(file->getFile()->fileName());
		Util::logMessage("Sending " + file->getFile()->fileName());
        	Util::logMessage("File block size = " + QString::number(block.size()));
        	this->sendBytesToClient(block);
		Util::logMessage("File sent");
		return true;
	}
	return false;
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
	QDataStream in(this);
	forever{
		if(this->nextBlockSize == 0){
			if(this->bytesAvailable() >= sizeof(qint64)){
				in >> this->nextBlockSize;	
			}
		}
		if(this->bytesAvailable() == this->nextBlockSize){
			break;
		}

	}
	QByteArray block;
	in >> block;
	this->processBlock(block);
	this->nextBlockSize = 0;
}

void MumuConnection::processBlock(QByteArray block)
{
	QDataStream in(&block, QIODevice::ReadOnly);
	if(this->statusConnection == -1){
		if(Util::processMsg(block) == SOH){
			this->statusConnection = 1;
			this->sendAckToClient();
			Util::logMessage("Connection accepted");
		}
	}else if(this->statusConnection == 1){ // requesting fd
		if(Util::processMsg(block) == ENQ){
			Util::logMessage("Client request file");
			QByteArray block = this->files->at(0)->getFileDescriptor().getBlockFileDescriptor();
			this->sendBytesToClient(block);
			this->statusConnection = 2;
		}
	}else if(this->statusConnection == 2){ 
		if(Util::processMsg(block) == ACK){ // FD Ok. Send file
			if(this->sendFile()){
				this->statusConnection = 3; // All blocks sent. 
			}
		}else{
			this->sendFileDescriptor();
		}
	}else if(this->statusConnection == 3){ // file sent. ok?
		if(Util::processMsg(block) == ACK){
			Util::logMessage("File is ok in client");
			this->statusConnection = 4;
		}else{
			Util::logMessage("File is not ok in client");
		}
	}
}

void MumuConnection::sendFileDescriptor()
{
	if(this->files->size() > 0){
		MumuFile * file = this->files->at(0);
		this->sendBytesToClient(file->getFileDescriptor().getBlockFileDescriptor());
		Util::logMessage("FD sent");
	}else{
		Util::logMessage("No file");
	}
}

void MumuConnection::sendBytesToClient(QByteArray data)
{
	Util::sendBytesTo(data,this);
}


void MumuConnection::sendMsgToClient(quint16 msg)
{
	Util::sendMsgTo(msg,this);	
}

void MumuConnection::sendAckToClient()
{
	this->sendMsgToClient(ACK);
}

void MumuConnection::sendNakToClient()
{
	Util::logMessage("Enviando NAK");
	this->sendMsgToClient(NAK);
}
