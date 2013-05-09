
#include "mumuserver.h"
#include <iostream>
#include <QDir>
#include <QTcpSocket>

/** Constructor */
MumuServer::MumuServer(QDir homeApp,int port, QObject *parent) : QTcpServer(parent) 
{
	this->blackListFile;
	this->blackListFile << "." << "..";
	this->totalSplit = 3;
	/* First - Look for a files in the homeapp/file and split it*/
	this->homeApp = homeApp;
	this->openAndSplitFile();


	if(this->listen(QHostAddress::Any, port)){
		std::cout<<"The MumuServer is listening any ip address on port " << this->serverPort() << std::endl;
	}
	if(this->isListening()){
		Util::logMessage("The server is listening");
	}
	connect(this, SIGNAL(newConnection()),this,SLOT(clientConnecting()));

	Util::logMessage("Constructor done!.");
}

void MumuServer::clientConnecting()
{
	Util::logMessage("Client wants connect!");
	std::cout << "Total clients connected = " << connections.size() << std::endl;
}

void MumuServer::sendFile(QString path)
{
	this->filePath = path;
} 

void MumuServer::openAndSplitFile()
{
	this->openFiles();
	/* Split the file */
	QDir blockDir = FileHandle::getPublicUserHome();
	if(blockDir.exists()){
		for(MumuFile * file : files){
			int countBlock = 1;
			QList<MumuBlock> blocks = file->getBlocks();
			for(QByteArray block : blocks){
				QString path = FileHandle::getUserHome() + "/block-" + QString::number(countBlock);
				std::cout << path.toStdString() << "block size = " << block.size() << std::endl;
				QFile fileBlock(path);
				fileBlock.open(QIODevice::WriteOnly);
				QDataStream out(&fileBlock);
				out << block;
				fileBlock.close();
				countBlock++;
			}
			std::cout << file->fileName().toStdString() << ". Block ready! " << std::endl;	
		}
	}
}

void MumuServer::openFiles()
{
	QDir fileDir(QDir::toNativeSeparators(homeApp.path() + "/file"));
	std::cout << fileDir.path().toStdString() << std::endl;
	if(fileDir.exists()){ // there is files directory in the application home dir
		std::cout << "fileDir exists" << std::endl;
		QStringList filesList = fileDir.entryList();
		for(int index = 0; index < filesList.size(); index++){
			QString fileName = filesList.at(index);
			if(this->blackListFile.contains(fileName)){
				continue;
			}
			QString path = fileDir.path() + "/" + fileName;
			std::cout << path.toStdString() << std::endl;
			MumuFile * file = new MumuFile(QDir::toNativeSeparators(path));
			if(file->exists()){
				files.append(file);
			}
		}
		std::cout << this->files.size() << " files found" << std::endl;
	}
}

void MumuServer::incomingConnection(int socketDescription)
{
	Util::logMessage("Incoming connection...");
	MumuConnection * connection = new MumuConnection(socketDescription,&(this->files),this);
	Util::logMessage("connection created");
	connection->setId("TESTE");
	connections.append(connection); 
	Util::logMessage("Leaving incoming connection...");
}

void MumuServer::showErrorMessage(QAbstractSocket::SocketError socketError)
{
	std::cout<<"ERROR: "<<socketError<<std::endl;
}

void MumuServer::socketStateChanged(QAbstractSocket::SocketState state)
{
	std::cout<<"Socket state = "<<state<<std::endl;
}

int MumuServer::getNumberOfParts()
{
	int parts = 0;

	for (MumuFile *file : files)
		parts += file->getSize();

	return parts;
}
