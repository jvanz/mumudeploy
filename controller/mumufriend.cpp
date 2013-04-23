#include "mumufriend.h"
#include "databasemanager.h"

MumuFriend::MumuFriend(QObject *parent)
	: QObject(parent)
{
}

void MumuFriend::sendFile(QString path)
{
	this->server = NetworkManager::getInstance(path,this->parent());
	this->server->sendFile(path);	
}

QSqlQuery MumuFriend::returnOpenProcess()
{
	DatabaseManager manager;
	return manager.returnOpenProcesses();
}
