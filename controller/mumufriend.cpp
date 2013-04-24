#include "mumufriend.h"
#include "databasemanager.h"

MumuFriend::MumuFriend(QObject *parent)
	: QObject(parent)
{
}

bool MumuFriend::sendFile(QString ip, QString path)
{
	DatabaseManager manager;

	this->server = NetworkManager::getInstance(path,this->parent());
	this->server->sendFile(path);

	return manager.insertNewProcess(ip, path, "S", this->server->getNumberOfParts());
}

QSqlQuery MumuFriend::returnOpenProcess()
{
	DatabaseManager manager;
	return manager.returnOpenProcesses();
}

void MumuFriend::updateSendPieces(QString ip, QString file)
{
	DatabaseManager manager;
	manager.updateSentReceive(ip, file);
}
