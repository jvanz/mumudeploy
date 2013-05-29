
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
	this->openFile();
	this->sendGreeting();
	
}

void MumuClient::sendGreeting()
{
	this->sendMsgToServer(SOH); // SOH - It's initial char of a new connection
	statusConnection = 1;
}

void MumuClient::readFile()
{
	QDataStream in(&tcpSocket);
	if(this->nextBlockSize == 0){
		if(tcpSocket.bytesAvailable() >= sizeof(qint64)){
			in >> this->nextBlockSize;	
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

	if(statusConnection == 1){ //waiting ack to greeting from server
		if(Util::processMsg(block) == ACK){
			Util::logMessage("Server accepted the connection");
			this->requestFilesToServer();
		}
	}else if(statusConnection == 2){ // waiting file descriptor from server
		if(block.size() > 2){ //recive fd
			Util::logMessage("Reciving FD");
			this->file = new MumuFile(block);
			this->sendAckToServer();
			this->statusConnection = 3;
		}else if(Util::processMsg(block) == NAK){ // probabli nak
			Util::logMessage("Server did not send FD");
		}
	}else if(this->statusConnection == 3){ // wainting the file
		if(block.size() > 2){ // recive file
			Util::logMessage("Receving the file");
			this->statusConnection = 4;
			if(this->processFileBlock(block)){
				this->sendAckToServer();
			}else{
				this->sendNakToServer();
			}
		}else{
			//TODO
		}
		this->sendAckToServer();
	}
}

bool MumuClient::processFileBlock(QByteArray block)
{
	if(this->file){
		QDir dir(FileHandle::getUserHome() +"/recive/"+ file->fileName());
		return Util::saveBlockLikeFile(dir, block, QString::number(this->currentBlock));
	}
	return false;
}

void MumuClient::sendBytesToServer(QByteArray data)
{
	Util::sendBytesTo(data,&(this->tcpSocket));
}

void MumuClient::sendAckToServer()
{
	this->sendMsgToServer(ACK);
}

void MumuClient::sendNakToServer()
{
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
