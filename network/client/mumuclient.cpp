
#include "mumuclient.h"
#include <iostream>
#include <QHostAddress>
#include <QDir>

MumuClient::MumuClient(QString path, QObject *parent) : filePath(path), tcpSocket(parent)
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
	QByteArray block;
	QDataStream out(&block,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << quint16(0) << msg;
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16));
	int bytesWriten = tcpSocket.write(block);
	Util::logMessage(QString::number(block.size()));
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
//	openFile();
	this->sendGreeting();
	
}

void MumuClient::sendGreeting()
{
	QByteArray msg = "GREETING";
	this->sendMsgToServer(msg);
	tcpSocket.flush();
	statusConnection = 1;
}

void MumuClient::readFile()
{
/*
	std::cout<<"Receiving data"<<std::endl;
	buffer.clear();
	QDataStream in(&tcpSocket);
	int blockSize = tcpSocket.bytesAvailable();
	char * bytes = new char[blockSize];
	in.readRawData(bytes, blockSize);	
	std::cout << "Block size = " << blockSize << std::endl;
	inFile->writeRawData(bytes,blockSize);
*/	
	QDataStream in(&tcpSocket);
	in.setVersion(QDataStream::Qt_4_3);
	forever {
		if (nextBlockSize == 0) {
			if (tcpSocket.bytesAvailable() < sizeof(quint16)){
				break;
			}
			in >> nextBlockSize;
			Util::logMessage(QString::number(nextBlockSize));
		}
		if (nextBlockSize == 0xFFFF) {
			Util::logMessage("0xFFFF encontrado");
			break;
		}
		if (tcpSocket.bytesAvailable() < nextBlockSize){
			Util::logMessage("nextblocksize maior que bytes disponiveis");
			break;
		}
		Util::logMessage(QString::number(tcpSocket.bytesAvailable()));
		QString msg;
		in >> msg;
		Util::logMessage(msg);
		if(statusConnection == 1 & msg == "OK"){
			// Server is answer your greeting! Request the file
			statusConnection = 2;
			this->sendMsgToServer("FILE");
			
		}else if(statusConnection == 2){
			//Server is sending the response about the file
		}
		nextBlockSize = 0;
	}

}
bool MumuClient::openFile()
{
	//QString pathFile = QDir::homePath() + "/client/twd.mvk";
	//std::cout << pathFile.toStdString() << std::endl;
	file = new QFile(QDir::toNativeSeparators(filePath));
	//std::cout << pathFile.toStdString() << std::endl;
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
