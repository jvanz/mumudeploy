
#include "mumuserver.h"
#include <iostream>
#include <QDir>
#include <QTcpSocket>

/** Constructor */
MumuServer::MumuServer(int port, QObject *parent) : QTcpServer(parent) 
{
	this->blackListFile;
	this->blackListFile << "." << "..";
	this->totalSplit = 3;
	/* First - Look for a files in the homeapp/file and split it*/
	this->openAndSplitFile();
	this->databaseManager = DatabaseManager::getInstance();


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
	QString path = QDir::currentPath();

	this->openFiles();
	/* Split the file */
	if(QDir::setCurrent(FileHandle::getUserHome())){
		for(MumuFile * file : files){
			QDir::current().mkdir(file->fileName());
			QDir blockDir(QDir::currentPath() +"/" + file->fileName());
			int countBlock = 1;
			QList<MumuBlock> blocks = file->getBlocks();
			for(QByteArray block : blocks){
				Util::saveBlockLikeFile(blockDir, block, QString::number(countBlock));
				countBlock++;
			}
			std::cout << file->fileName().toStdString() << ". Block ready! " << std::endl;	
		}
	}

	QDir::setCurrent(path);
}

void MumuServer::openFiles()
{
	if(QDir::setCurrent(FileHandle::getPublicUserHome().path() + "/file")){ // there is files directory in the application home dir
		QDir fileDir(QDir::currentPath());
		QStringList filesList = fileDir.entryList();
		for(int index = 0; index < filesList.size(); index++){
			QString fileName = filesList.at(index);
			if(this->blackListFile.contains(fileName)){
				continue;
			}
			std::cout << fileName.toStdString() << std::endl;
			MumuFile * file = new MumuFile(fileName);
			if(file->exists()){
				files.append(file);
			}
		}
		std::cout << this->files.size() << " files found" << std::endl;
		QDir::setCurrent(FileHandle::getDirUserHome().path());
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
	
bool MumuServer::insertNewProcess(QString ip, QString file)
{
	for(MumuFile * mumuFile : this->files){
		if(mumuFile->fileName() == file){
			return this->databaseManager->insertNewProcess(ip, file, "S", mumuFile->getTotalBlocksCount());
		}
	}
	return false;
}
