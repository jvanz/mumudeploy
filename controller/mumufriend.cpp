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

QString MumuFriend::getDestinationDir()
{
	DatabaseManager manager;
	return manager.getDestinationDir();
}

QString MumuFriend::getServerAddress()
{
	DatabaseManager manager;
	return manager.getServerAddress();
}

void MumuFriend::updateDestDir(QString path)
{
	DatabaseManager manager;
	manager.updateDestDir(path);
}

void MumuFriend::updateServerAddress(QString address)
{
	DatabaseManager manager;
	manager.updateServerAddress(address);
}

QObject *MumuFriend::retornaPendentesModel()
{
	DatabaseManager manager;
	return manager.retornaPendetesModel();
}
