
#include "mumuserver.h"
#include <iostream>
#include <QDir>
#include <QTcpSocket>

/** Constructor */
MumuServer::MumuServer(QDir homeApp,QObject *parent) : QTcpServer(parent) 
{
	this->blackListFile;
	this->blackListFile << "." << "..";
	this->totalSplit = 3;
	/* First - Look for a files in the homeapp/file and split it*/
	this->homeApp = homeApp;
	this->openAndSplitFile();

/*
	if(this->listen(QHostAddress::Any, 8080)){
		std::cout<<"The MumuServer is listening any ip address on port " << this->serverPort() << std::endl;
	}
	if(this->isListening()){
		std::cout<<"The server is listening"<<std::endl;
	}
	connect(this, SIGNAL(newConnection()),this,SLOT(clientConnecting()));
*/
	std::cout<<"Constructor done!."<<std::endl;
}

void MumuServer::clientConnecting()
{
	std::cout<<"Client wants connect!"<<std::endl;
	std::cout << "Total clients connected = " << connections.size() << std::endl;
}


void MumuServer::openAndSplitFile()
{
	this->openFiles();
	/* Split the file */
	QDir blockDir(QDir::toNativeSeparators(homeApp.path() + "/block"));
	if(blockDir.exists()){
		std::cout << "blockDir exists" << std::endl;
		std::cout << files.size() << " files to split" << std::endl;
		
		for(int index = 0; index < files.size(); index++){
			QFile * file = files.at(index);
			std::cout << "File size = " << file->size() << std::endl;
			int countBlock = 1;
			for(QByteArray block : this->splitFile(file, this->totalSplit)){
				std::cout << "block = " << countBlock << std::endl;
				QString path = QDir::toNativeSeparators(blockDir.path() + "/" + file->fileName() + "-" + countBlock);
				QFile fileBlock(path);
				QDataStream out(&fileBlock);
				out << block;
				fileBlock.close();
			}
			std::cout << file->fileName().toStdString() << ". Block ready! " << std::endl;	
		}
	}
}


QList<QByteArray> MumuServer::splitFile(QFile* file, int blockCount)
{
	std::cout << "entrou no splitFile. File name = " << file->fileName().toStdString() << std::endl;
	QList<QByteArray> blocks;
	qint64 fileSize = file->size();
	qint64 blockSize = fileSize / blockCount;
	qint64 restBytes = fileSize % blockCount;
	while(blocks.count() < blockCount){
		QByteArray block = file->read(blockSize);
		if(block.isEmpty()){ // There are not more bytes avaiable
			break;
		}
		blocks.append(block);
		file->seek((blocks.count() * blockSize) + 1);
	}
	if(restBytes){
		QByteArray block = file->read(blockSize);
		if(!block.isEmpty()){ // There are not more bytes avaiable
			blocks.append(block);
		}
	}
	std::cout << file->fileName().toStdString() << " splited " << std::endl;
	return blocks;
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
			QFile * file = new QFile(QDir::toNativeSeparators(path));
			if(file->exists()){
				files.append(file);
			}
		}
		std::cout << this->files.size() << " files found" << std::endl;
	}
}

void MumuServer::incomingConnection(int socketDescription)
{
	std::cout<<"Incoming connection..."<<std::endl;
	MumuConnection * connection = new MumuConnection(socketDescription,this);
	std::cout<<"connection created"<<std::endl;
	connection->setId("TESTE");
	connections.append(connection); 
	std::cout<<"Leaving incoming connection..."<<std::endl;
}

void MumuServer::showErrorMessage(QAbstractSocket::SocketError socketError)
{
	std::cout<<"ERROR: "<<socketError<<std::endl;
}

void MumuServer::socketStateChanged(QAbstractSocket::SocketState state)
{
	std::cout<<"Socket state = "<<state<<std::endl;
}
