
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
		qDebug() <<"The MumuServer is listening any ip address on port " << this->serverPort();
	}
	if(this->isListening()){
		qDebug() << "The server is listening";
	}
	connect(this, SIGNAL(newConnection()),this,SLOT(clientConnecting()));

	qDebug() << "Constructor done!.";
}

void MumuServer::clientConnecting()
{
	qDebug() << "Client wants connect!";
	qDebug() << "Total clients connected = " << connections.size();
}

void MumuServer::sendFile(QString path)
{
	this->filePath = path;
} 

void MumuServer::openAndSplitFile()
{
	QString currPath = QDir::currentPath();

	this->openFiles();
	/* Split the file */
	for(MumuFile * file : files){
		FileHandle::getDirUserHome().mkdir(file->fileName());
		QDir blockDir(FileHandle::getUserHome() +"/" + file->fileName());
		int countBlock = 1;
		QList<MumuBlock> blocks = file->getBlocks();
		for(QByteArray block : blocks){
			Util::saveBlockLikeFile(blockDir, block, QString::number(countBlock));
			countBlock++;
		}
		qDebug() << file->fileName() << ". Block ready! ";	
	}

	QDir::setCurrent(currPath);
}

void MumuServer::openFiles()
{
	QDir fileDir(FileHandle::getPublicUserHome().path() + "/file");
	qDebug() << fileDir.path();
	QStringList filesList = fileDir.entryList();
	for(int index = 0; index < filesList.size(); index++){
		QString fileName = filesList.at(index);
		if(this->blackListFile.contains(fileName)){
			continue;
		}
		qDebug() << fileName;
		MumuFile * file = new MumuFile(FileHandle::getPublicUserHome().path() + "/" + fileName);
		qDebug() << FileHandle::getPublicUserHome().path() + "/" + fileName;
		files.append(file);
	}
	qDebug() << this->files.size() << " files found";
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
	return true;
}
