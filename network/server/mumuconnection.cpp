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
	this->dbManager = DatabaseManager::getInstance();
	this->lastBlockOk = true;
	this->currentFile = NULL;
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
	if(this->statusConnection == -2){ // enviar último FD novamente;
		this->sendFileDescriptor();
		this->statusConnection = 1;
		return true;
	}
	if(!this->lastBlockOk){
		this->sendBlockToClient(this->currentBlock);
		this->lastBlockOk = true;
		return true;
	}
	if(this->currentFile){ //sending a file
		int blockNumber = this->dbManager->nextPiece(this->currentFile->fileName(), clientIP.toString());
		if(blockNumber <= this->currentFile->getTotalBlocksCount()){
			this->currentBlock = Util::loadFileBlock(FileHandle::getDirUserHome(), this->currentFile->fileName(), blockNumber);
			this->sendBlockToClient(this->currentBlock);
			return true;
		}else{
			this->currentFile = NULL;
		}
		
	}  
	for(int index = 0; index < this->files->size(); index++){
		MumuFile * file = this->files->at(index);
		int blockNumber = this->dbManager->nextPiece(file->fileName(), clientIP.toString());
		if(blockNumber < file->getTotalBlocksCount()){
			this->currentFile = file; 
			this->sendFileDescriptor();
			this->statusConnection = 1;
			return true;
		}
	}
	this->sendMsgToClient(EOT);
	this->statusConnection = 3;
	return true;	
}

void MumuConnection::processData()
{
	QDataStream in(this);
	in.setVersion(QDataStream::Qt_4_3);
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
	quint16 msg;
	in >> msg;		
	if(this->statusConnection == -1){
		if(msg == SOH){
			QHostAddress ip;
			in >> ip;
			if(this->registreIP(ip)){
				this->statusConnection = 1; // connected. Start file sending
				this->sendFile();
			}else{
				this->sendNakToClient();
			}
		}
	}else if(this->statusConnection == 1){
		if(msg == NAK){ // fd ok
			this->statusConnection = -2;	
		}else{
			this->statusConnection = 2;
		}
		this->sendFile();
	}else if(this->statusConnection == 2){ // sending file
		if(msg == ACK){ // last block ok
			this->dbManager->updateSentReceive(this->clientIP.toString(),this->currentFile->fileName());
		}else{ // send again last block
			this->lastBlockOk = false;
		}
		this->sendFile();
	}else if(this->statusConnection == 3){ // no more files to send. 
		this->sendMsgToClient(EOT);
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
	this->sendMsgToClient(NAK);
}
	
bool MumuConnection::registreIP(QHostAddress ip)
{
	for(int index = 0; index < this->files->size(); index++){
		MumuFile * file = this->files->at(index);
		this->dbManager->insertNewProcess(ip.toString(),file->fileName() ,"S", file->getFileDescriptor().getTotalBlocksCount());
		Util::logMessage(ip.toString() + " - " + file->fileName());
	}
	this->clientIP = ip;
	return true;
	
}
	
bool MumuConnection::sendFileDescriptor()
{
	if(this->currentFile){
		QByteArray tmpBlock;
		QDataStream in(&tmpBlock, QIODevice::WriteOnly);
		in.setVersion(QDataStream::Qt_4_3);
		in << ENQ << this->currentFile->getFileDescriptor().getBlockFileDescriptor();
		this->sendBytesToClient(tmpBlock);
		return true;
	}
	return false;	
}
	
void MumuConnection::sendBlockToClient(QByteArray block)
{
	if(this->currentFile){
		QByteArray tmpBlock;
		QDataStream in(&tmpBlock, QIODevice::WriteOnly);
		in.setVersion(QDataStream::Qt_4_3);
		in << STX << block;
		this->sendBytesToClient(tmpBlock);
	}
}
