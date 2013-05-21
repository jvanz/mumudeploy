
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
	statusConnection = -1;
}

/**
 * This function conects the MumuClient to the MumuServer
 */
bool MumuClient::connectMumuServer()
{
	tcpSocket.connectToHost(ipServer, portServer);
	return tcpSocket.isOpen();
}

void MumuClient::sendMsgToServer(char msg)
{
	Util::logMessage("Enviando mensagem");
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
	this->sendMsgToServer(quint8(SOH)); // SOH - It's initial char of a new connection
	statusConnection = 1;
}

void MumuClient::readFile()
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	QDataStream in(&this->tcpSocket);
	in.setVersion(QDataStream::Qt_4_3);
	
	forever{
		if(nextBlockSize == 0){
			if(this->tcpSocket.bytesAvailable() < sizeof(quint16)){
				break;	
			}
			in >> nextBlockSize;
		}

		if (nextBlockSize == 0xFFFF) {
			break;
		}

		if(this->tcpSocket.bytesAvailable() < nextBlockSize){
			break;
		}
	}
	out << in;
	this->processBlock(block);
}

void MumuClient::processBlock(QByteArray block)
{
	Util::logMessage("Bloco para processar");
	Util::logMessage("Tamanho = " + QString::number(block.size()));
}

bool MumuClient::processFileDescriptorBlock(QByteArray * block)
{
	Util::logMessage("Processing FD");
	Util::logMessage(QString::number(block->size()));
	QDataStream in(block, QIODevice::ReadOnly);
	QString  fileName;
	quint8 blocksCount;
	QByteArray md5;

	in >> fileName >> blocksCount >> md5;

	Util::logMessage(fileName);
	Util::logMessage(QString::number(blocksCount));
	Util::logMessage(QString(md5.toHex()));
	
	Util::logMessage("FD processed");
	return true;
}

void MumuClient::sendBytesToServer(char * data)
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
	file = new QFile(QDir::toNativeSeparators(filePath));
	bool isOpen = file->open(QIODevice::WriteOnly);
	std::cout << "File size = " << file->size() << std::endl;	
	inFile = new QDataStream(file);
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
