
#include "mumuclient.h"
#include <iostream>
#include <QHostAddress>
#include <QDir>

MumuClient::MumuClient(QString path,QHostAddress ip,int port, QObject *parent) : filePath(path), ipServer(ip), portServer(port), tcpSocket(parent)
{
	connect(&tcpSocket,SIGNAL(readyRead()),this,SLOT(readFile()));
	connect(&tcpSocket,SIGNAL(disconnected()),this,SLOT(closeStream()));
	connect(&tcpSocket,SIGNAL(connected()),this,SLOT(serverConnected()));
	connect(&tcpSocket,SIGNAL(hostFound()),this,SLOT(hostServerFound()));
	connect(&tcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(socketStateChanged(QAbstractSocket::SocketState)));
	connect(&tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(catchError(QAbstractSocket::SocketError)));
	connect(&tcpSocket,SIGNAL(proxyAuthenticationRequired(const QNetworkProxy & proxy, QAuthenticator * authenticator )),this,SLOT(proxyAuthentication(const QNetworkProxy & proxy, QAuthenticator * authenticator)));
	connectMumuServer();
	this->statusConnection = -1;
	this->currentBlock = 1;
	this->nextBlockSize = 0;
	this->buffer.clear();
}

/**
 * This function conects the MumuClient to the MumuServer
 */
bool MumuClient::connectMumuServer()
{
	tcpSocket.connectToHost(ipServer, portServer);
	return tcpSocket.isOpen();
}

void MumuClient::sendMsgToServer(quint16 msg)
{
	Util::sendMsgTo(msg,&(this->tcpSocket));	
}

void MumuClient::catchError(QAbstractSocket::SocketError error)
{
	std::cout << "ERROR! -> " << error << std::endl;
}

void MumuClient::proxyAuthentication(const QNetworkProxy & proxy, QAuthenticator * authenticator)
{
	std::cout << "proxy authentication" << std::endl;
}

void MumuClient::serverConnected()
{
	std::cout<<"Client connected!"<<std::endl;
	connected = true;
	nextBlockSize = 0;
	this->sendGreeting();
	
}

void MumuClient::sendGreeting()
{
	QByteArray tmpBlock;
	QDataStream out(&tmpBlock,QIODevice::WriteOnly);
	out << SOH << this->tcpSocket.localAddress();
	this->sendBytesToServer(tmpBlock); // SOH - It's initial char of a new connection
	statusConnection = 1;
}

void MumuClient::readFile()
{
	QDataStream in(&tcpSocket);
	if(this->nextBlockSize == 0){
		if(tcpSocket.bytesAvailable() >= sizeof(qint64)){
			in >> this->nextBlockSize;	
			Util::logMessage("NEXTBLOCKSIZE = " + QString::number(this->nextBlockSize));
		}
	}
	if(tcpSocket.bytesAvailable() == this->nextBlockSize){
		QByteArray block;
		in >> block;
		this->processBlock(block);
		this->nextBlockSize = 0;
	}
}

void MumuClient::processBlock(QByteArray block)
{
	Util::logMessage("----------------------------------------------------------------");
	Util::logMessage("BLOCK RECIVE SIZE = " + QString::number(block.size()));
	QDataStream in(&block, QIODevice::ReadOnly);
	QByteArray tmpBlock;
	quint16 msg;
	in >> msg;		
	if(this->statusConnection == 1){
		if(msg == ENQ){ // server sending the first FD;
			Util::logMessage("ENQ recebido");
			in >> tmpBlock;
			this->processFileDescriptorBlock(tmpBlock);
			this->sendAckToServer();
			this->statusConnection = 2;
		}else if(msg == NAK){
			Util::logMessage("Connection denied");
		}
	}else if(this->statusConnection == 2){ // reciving blocks
		Util::logMessage("entrou no status 2");
		if(msg == STX ){
			Util::logMessage("STX recebido");
			in >> tmpBlock;
			Util::logMessage("TMPBLOCK SIZE = " + QString::number(tmpBlock.size()));
			if(this->processFileBlock(tmpBlock)){
				this->sendAckToServer();
			}else{
				this->sendNakToServer();
			}
		}else if(msg == EOT){ // no more files
			Util::logMessage("No more files to recive");
			Util::recreateFiles();
		}
	}
}

bool MumuClient::processFileBlock(QByteArray block)
{
	Util::logMessage("process file block");
	QDir dir(FileHandle::getUserHome() + "/" + this->currentFile->fileName());
	if(Util::saveBlockLikeFile(dir, block, QString::number(this->currentBlock))){
		this->currentBlock++;
		return true;
	}
	return false;
}
			
bool MumuClient::processFileDescriptorBlock(QByteArray tmpBlock)
{
	Util::logMessage("Tratando FD");
	FileDescriptor * fd = FileDescriptor::processFileDescriptorBlock(tmpBlock);
	QDir dir = FileHandle::getDirUserHome();
	Util::logMessage(dir.path());
	if(!dir.exists(fd->getFileName())){
		dir.mkdir(fd->getFileName());
	}
	this->currentFile = new MumuFile(*fd);	
	this->currentBlock = 1;
	return true;
}

void MumuClient::sendBytesToServer(QByteArray data)
{
	Util::sendBytesTo(data,&(this->tcpSocket));
}

void MumuClient::sendAckToServer()
{
	Util::logMessage("enviando ack");
	this->sendMsgToServer(ACK);
}

void MumuClient::sendNakToServer()
{
	Util::logMessage("enviando nak");
	this->sendMsgToServer(NAK);
}

void MumuClient::requestFilesToServer()
{
	this->sendMsgToServer(ENQ);
	statusConnection = 2; // request statius
	Util::logMessage("File requested");
}

bool MumuClient::openFile()
{
	file = new MumuFile(QDir::toNativeSeparators(filePath));
	bool isOpen = file->open(QIODevice::WriteOnly);
	std::cout << "File size = " << file->size() << std::endl;	
	inFile = new QDataStream(file->getFile());
	return isOpen;
}

void MumuClient::closeStream()
{
	if(statusConnection != 3){
		return;
	}
	std::cout << "Closing stream" << std::endl;
	if(file){
		file->flush();
		file->close();
	}
}

void MumuClient::hostServerFound()
{
	std::cout << "Host Server!" << std::endl;
}

void MumuClient::socketStateChanged(QAbstractSocket::SocketState state)
{
	std::cout << "Socket state = " << state << std::endl;
}
