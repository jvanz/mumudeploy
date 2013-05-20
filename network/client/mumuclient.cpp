
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
	out << qint64(0) << msg;
	out.device()->seek(0);
	out << qint64(block.size() - sizeof(qint64));
	tcpSocket.write(block);
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
	Util::logMessage("Client has data to process");
	QDataStream in(&tcpSocket);
	in.setVersion(QDataStream::Qt_4_3);

	forever{
		Util::logMessage("Bytes available = " + QString::number(tcpSocket.bytesAvailable()));
		if (nextBlockSize == 0) {
			if (tcpSocket.bytesAvailable() < sizeof(quint64)){
				break;
			}
			in >> nextBlockSize;
			Util::logMessage("nextBlockSize = " + QString::number(nextBlockSize));
		}
		if (tcpSocket.bytesAvailable() == nextBlockSize){
			Util::logMessage("tcpSocket.bytesAvailable() == nextBlockSize)");
			break;
		}
	} // forever
	nextBlockSize = qint64(0);
		if(statusConnection == 1){
			QString msg;
			in >> msg;
			Util::logMessage(msg);
			if(msg == "OK"){
				// Server is answer your greeting! Request the file
				statusConnection = 2;
				this->sendMsgToServer("FILE");
				nextBlockSize = 0;
			}
		}else if(statusConnection == 2){
			//Server is sending the response about the file
			if(this->openFile()){
				Util::logMessage("Recebendo arquivo!");
				Util::logMessage("Bytes available = " + QString::number(tcpSocket.bytesAvailable()));
				QByteArray block;
				in >> block;
				Util::logMessage("Block = " + QString::number(block.size()) + " bytes");
				inFile->writeRawData(block.constData(),nextBlockSize);
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
