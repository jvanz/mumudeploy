
#include "mumuclient.h"
#include <iostream>
#include <QHostAddress>
#include <QDir>

MumuClient::MumuClient(QObject *parent) : tcpSocket(parent)
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
	tcpSocket.connectToHost(QHostAddress::LocalHost, 8080);
	std::cout<<"TCP Socket state? "<<tcpSocket.state()<<std::endl;
	return tcpSocket.isOpen();
}

void MumuClient::sendOk()
{
	this->sendMsgToServer("OK");
}

void MumuClient::sendMsgToServer(QString msg)
{
	std::cout << "Sending to server: " << msg.toStdString() << std::endl;
	QByteArray block = msg.toUtf8();
	QDataStream out(&block,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << quint16(0) << block;
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16));
	int bytesWriten = tcpSocket.write(block);
	std::cout << "Bytes writen = " << bytesWriten << std::endl;
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
	this->sendGreeting();
	
}

void MumuClient::sendGreeting()
{
	QByteArray msg = "GREETING";
	if(tcpSocket.write(msg)){
		std::cout << "Greeting sent!" << std::endl;
		tcpSocket.flush();
		statusConnection = 1;
	}
}

void MumuClient::readFile()
{
	std::cout<<"Receiving data"<<std::endl;
	buffer.clear();
	QDataStream in(&tcpSocket);
	int blockSize = tcpSocket.bytesAvailable();
	char * bytes = new char[blockSize];
	in.readRawData(bytes, blockSize);	
	openFile();
	std::cout << "Block size = " << blockSize << std::endl;
	inFile->writeRawData(bytes,blockSize);
	this->closeStream();
}

bool MumuClient::openFile()
{
	QString pathFile = QDir::homePath() + "/client/teste";
	std::cout << pathFile.toStdString() << std::endl;
	file = new QFile(QDir::toNativeSeparators(pathFile));
	std::cout << pathFile.toStdString() << std::endl;
	bool isOpen = file->open(QIODevice::WriteOnly);
	std::cout << "File size = " << file->size() << std::endl;	
	inFile = new QDataStream(file);
	return isOpen;
}

void MumuClient::closeStream()
{
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
