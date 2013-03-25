
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
	int bytesWriten = tcpSocket.write(msg.toUtf8());
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
	if(statusConnection != 2){
		QByteArray buffer = tcpSocket.readAll();
		QString msg(buffer);
		std::cout << "Message: " << msg.toStdString() << std::endl; 
		if(msg == "FILE"){
			this->sendOk();
			statusConnection = 2;
		}
	}else{
		//Receving file
		if(this->openFile()){
			std::cout<<"Bytes Available = "<<tcpSocket.bytesAvailable()<<std::endl;
			buffer = tcpSocket.read(tcpSocket.bytesAvailable());
			std::cout << "Buffer Bytes = " << buffer.size() << std::endl;
			QDataStream in(file);
			in << buffer;
		}
	}
	
}

bool MumuClient::openFile()
{
	QString pathFile = QDir::homePath() + "/client/1-04 Stairway To Heaven.m4a";
	std::cout << pathFile.toStdString() << std::endl;
	file = new QFile(QDir::toNativeSeparators(pathFile));
	std::cout << file << std::endl;
	if(file->exists()){ // File exists
		std::cout << "File opened!" << std::endl;
	}	
	return file->open(QIODevice::WriteOnly);
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
