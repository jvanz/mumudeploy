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
	connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)));
	connect(this,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(socketStateChanged(QAbstractSocket::SocketState)));
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
	QByteArray block = msg.toUtf8();
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        out << quint16(0) << block;
        out.device()->seek(0);
        out << quint16(block.size() - sizeof(quint16));
	int bytesWriten = write(block);
	flush();
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

/**
 * This funtion send the file for client. When we write de file on the socket is necessary use writeRawData because this is the only
 * way to write just the bytes on the socket. Otherwise, the QT will serialize the whole object.
 */ 
void MumuConnection::sendFile()
{
	if(file){
		QByteArray block;
		block.clear();
		QDataStream out(&block,QIODevice::WriteOnly);
		QByteArray blockFile = file->readAll();
		std::cout << "Block file = " << blockFile.size() << std::endl;
		std::cout << "File size = " << file->size() << std::endl;
        	out.writeRawData(blockFile.constData(),blockFile.size());
		int bytesWriten = write(block);
		std::cout << "Bytes writen = " << bytesWriten << std::endl;
	}
}

void MumuConnection::openFile()
{
	QString pathFile = QDir::homePath() + "/server/twd.mkv";
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
	this->openFile();
	this->sendFile();
}
