
#include "mumuclient.h"
#include <iostream>
#include <QHostAddress>
#include <QDir>

MumuClient::MumuClient(QString path,QHostAddress ip, QObject *parent) : filePath(path),ipServer(ip), tcpSocket(parent)
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
	tcpSocket.connectToHost(ipServer, 8080);
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
	QByteArray msg = "GREETING";
	this->sendMsgToServer(msg);
	tcpSocket.flush();
	statusConnection = 1;
}

void MumuClient::readFile()
{
/*
	if(statusConnection == 2){
	std::cout<<"Receiving data"<<std::endl;
	buffer.clear();
	QDataStream in(&tcpSocket);
	int blockSize = tcpSocket.bytesAvailable();
	char * bytes = new char[blockSize];
	in.readRawData(bytes, blockSize);	
	std::cout << "Block size = " << blockSize << std::endl;
	inFile->writeRawData(bytes,blockSize);
*/	
	Util::logMessage("Client has data to process");
	QDataStream in(&tcpSocket);
	in.setVersion(QDataStream::Qt_4_3);
	if(statusConnection == 2){
		Util::logMessage("Reading file");
		int blockSize = tcpSocket.bytesAvailable();
		bytes = new char[blockSize];
		in.readRawData(bytes, blockSize);
		Util::logMessage("Quantidade bytes = " + QString::number(sizeof(blockSize)));
		QByteArray block = MumuFile::uncompress(QByteArray(bytes));
		inFile->writeRawData(block.constData(),block.size());
	}else{
		forever {
			if (nextBlockSize == 0) {
				if (tcpSocket.bytesAvailable() < sizeof(quint16)){
					continue;
				}
				in >> nextBlockSize;
				Util::logMessage(QString::number(nextBlockSize));
			}
			if (nextBlockSize == 0xFFFF) {
				Util::logMessage("0xFFFF encontrado");
				break;
			}
			if (tcpSocket.bytesAvailable() == nextBlockSize){
				Util::logMessage("nextblocksize maior que bytes disponiveis");
				break;
			}
//		nextBlockSize = 0;
		} // forever
		if(statusConnection == 1){
			Util::logMessage("primeiro if");
			QString msg;
			in >> msg;
			Util::logMessage(msg);
			if(msg == "OK"){
				// Server is answer your greeting! Request the file
				statusConnection = 2;
				this->sendMsgToServer("FILE");
				nextBlockSize = 0;
			}
		}else if(false /*statusConnection == 2*/){
			Util::logMessage("segundo if");
			//Server is sending the response about the file
			if(this->openFile()){
				Util::logMessage("Recebendo arquivo!");
				QByteArray block;
				in >> block;
				inFile->writeRawData(block.constData(),nextBlockSize);
				statusConnection = 3;
			}
		}
	}
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
