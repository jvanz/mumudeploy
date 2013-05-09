#include <iostream>
#include <QDir>

#include "mumuconnection.h"


MumuConnection::MumuConnection(int socketDescriptor,QList<MumuFile*>* fileList, QObject * parent) : files(fileList), QTcpSocket(parent)
{
	if(this->setSocketDescriptor(socketDescriptor)){
		std::cout << "Socket descriptor setted" << std::endl;
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
	std::cout << "Sending file to client" << std::endl;
	sendMsgToClient("FILE");
}

void MumuConnection::sendMsgToClient(QString msg)
{
	std::cout << "Sending to client: " << msg.toStdString() << std::endl;
	QByteArray block;
	QDataStream out(&block,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << quint16(0) << msg;
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16));
	int bytesWriten = write(block);
	Util::logMessage(QString::number(bytesWriten));
	
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
        	int bytesWriten = write(blockFile.constData(),blockFile.size());
		std::cout << "Bytes writen = " << bytesWriten << std::endl;
		file->getFile()->close();
		this->disconnectFromHost();
		Util::logMessage("File sent");
	}
/*		Util::logMessage("Sending file");
		MumuFile * file = files->at(0);
		Util::logMessage(file->fileName());
		file->getFile()->open(QIODevice::ReadOnly);	
		QByteArray blockFile = file->getFile()->readAll();
		Util::logMessage(QString("File size = ") + QString::number(blockFile.size()));
//		QByteArray compressFile = MumuFile::compress(blockFile);
		QByteArray block;
		QDataStream out(&block,QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_3);
		out << quint16(0) << compressFile << quint16(0xFFFF);
		out.device()->seek(0);
		out << quint16(block.size() - sizeof(quint16));
		int bytesWriten = write(block);
		Util::logMessage(QString::number(bytesWriten));
		Util::logMessage("File sent");
		file->getFile()->close();
		this->disconnectFromHost();	
		return;
*/
	
	Util::logMessage("File did not send");
	/* if(file){
		QByteArray block;
		block.clear();
		QDataStream out(&block,QIODevice::WriteOnly);
		QByteArray blockFile = file->readAll();
		std::cout << "Block file = " << blockFile.size() << std::endl;
		std::cout << "File size = " << file->size() << std::endl;
        	out.writeRawData(blockFile.constData(),blockFile.size());
		int bytesWriten = write(block);
		std::cout << "Bytes writen = " << bytesWriten << std::endl;
		this->disconnectFromHost();
	}
	*/
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
	std::cout << "Socket " << id.toStdString() << " has data to process. " << std::endl; 
	QDataStream in(this);
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	in.setVersion(QDataStream::Qt_4_3);
	forever{
		quint8 byte;
		in >> byte;
		if(byte == quint8(2)){ //STX - START OF TEXT ( FIRST BYTE )
			Util::logMessage("Achou STX");
			continue;
		}
		if(byte == quint8(3)){ // ETX - END OF TEXT ( LAST BYTE )
			Util::logMessage("Achou ETX");
			break;
		}
		out << byte;
	}

	this->processBlock(block);
}

void MumuConnection::processBlock(QByteArray block)
{
	QDataStream in(&block, QIODevice::ReadOnly);
	quint8 byte;
	in >> byte;
	Util::logMessage(QString::number(byte));
	if(byte == quint8(1)){
		Util::logMessage("Starting a connection");
		this->statusConnection = 1;
	}
}
	
void MumuConnection::sendMsgToClient(quint8 msg)
{
	QByteArray block;
	QDataStream out(&block,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << quint8(2) << msg << quint8(3);
	write(block.data());
	
}

void MumuConnection::sendAckToClient()
{
	this->sendMsgToClient(quint8(6));
}

void MumuConnection::sendNakToClient()
{
	this->sendMsgToClient(quint8(21));
}
