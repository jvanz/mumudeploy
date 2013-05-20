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
	out << qint64(0) << msg;
	out.device()->seek(0);
	out << qint64(block.size() - sizeof(qint64));
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
		QByteArray block;
		QDataStream out(&block,QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_3);
		out << qint64(0) << file->getFile()->readAll();
		out.device()->seek(0);
		out << qint64(block.size() - sizeof(qint64));
        	int bytesWriten = write(block);
		std::cout << "Bytes writen = " << bytesWriten << std::endl;
		file->getFile()->close();
	//	this->disconnectFromHost();
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
	Util::logMessage("Data to proccess");
	QDataStream in(this);
	in.setVersion(QDataStream::Qt_4_3);
	forever {
		if (nextBlockSize == 0) {
			if (this->bytesAvailable() < sizeof(quint64)){
				break;
			}
			in >> nextBlockSize;
		}
		if (nextBlockSize == 0xFFFF) {
			break;
		}
		if (this->bytesAvailable() < nextBlockSize){
			break;
		}
		Util::logMessage(QString::number(this->bytesAvailable()));
		QString msg;
		in >> msg;
		Util::logMessage(msg);
		
		if(msg == "GREETING"){
			Util::logMessage("Greeting received");
			// Client is saying hello! Give a response
			statusConnection = 1;
			this->sendMsgToClient("OK");
			
		}else if(statusConnection == 1 & msg == "FILE"){
			Util::logMessage("File requested");
			// client is requesting the files
			statusConnection = 2;
			this->sendFile();
		}
		nextBlockSize = 0;
	}
//	this->sendMsgToClient("Ola cliente");
//	this->openFile();
//	this->sendFile();

}
