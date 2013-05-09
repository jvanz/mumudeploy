
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

void MumuClient::sendOk()
{
	this->sendMsgToServer("OK");
}

void MumuClient::sendMsgToServer(QString msg)
{
	std::cout << "Sending to server: " << msg.toStdString() << std::endl;
	QByteArray block;
	QDataStream out(&block,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << quint8(2) << msg.data() << quint8(3); // STX - DATA - ETX
	Util::logMessage(QString::number(block.size()));
	int bytesWriten = tcpSocket.write(block.data());
}

void MumuClient::sendMsgToServer(quint8 msg)
{
	QByteArray block;
	QDataStream out(&block,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << quint8(2) << msg << quint8(3); // STX - DATA - ETX
	Util::logMessage(QString::number(block.size()));
	int bytesWriten = tcpSocket.write(block.data());
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
	this->sendMsgToServer(quint8(1)); // SOH - It's initial char of a new connection
	tcpSocket.flush();
	statusConnection = 1;
}

void MumuClient::readFile()
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	QDataStream in(&this->tcpSocket);
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
	//TODO - process block
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
