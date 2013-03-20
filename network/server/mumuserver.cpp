
#include "mumuserver.h"
#include <iostream>

/** Constructor */
MumuServer::MumuServer(QObject *parent) : QTcpServer(parent) 
{
	if(this->listen(QHostAddress::Any, 6666)){
		std::cout<<"The MumuServer is listeing any ip address on port 666."<<std::endl;
	}
	if(this->isListening()){
		std::cout<<"The server is listening"<<std::endl;
	}
	connect(this, SIGNAL(newConnection()),this,SLOT(clientConnecting()));
	std::cout<<"Constructor done!."<<std::endl;
}

void MumuServer::clientConnecting()
{
	std::cout<<"Client wants connect!"<<std::endl;
}


