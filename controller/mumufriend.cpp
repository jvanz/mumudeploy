#include "mumufriend.h"

MumuFriend::MumuFriend(QObject *parent)
	: QObject(parent)
{
}

void MumuFriend::sendFile(QString path)
{
	this->server = NetworkManager::getInstance(path,this->parent());
	this->server->sendFile(path);	
	
}
