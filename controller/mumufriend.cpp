#include "mumufriend.h"
#include "databasemanager.h"

MumuFriend::MumuFriend(QObject *parent)
	: QObject(parent)
{
	NetworkManager manager;
	this->server = manager.getInstance(6666, this->parent());
}

bool MumuFriend::insertNewProcess(QString ip, QString path)
{
	return this->server->insertNewProcess(ip, path);
}

QSqlQuery MumuFriend::returnOpenProcess()
{
	return DatabaseManager::getInstance()->returnOpenProcesses();
}

void MumuFriend::updateSendPieces(QString ip, QString file)
{
	DatabaseManager::getInstance()->updateSentReceive(ip, file);
}

QString MumuFriend::getDestinationDir()
{
	return DatabaseManager::getInstance()->getDestinationDir();
}

QString MumuFriend::getServerAddress()
{
	return DatabaseManager::getInstance()->getServerAddress();
}

void MumuFriend::updateDestDir(QString path)
{
	DatabaseManager::getInstance()->updateDestDir(path);
}

void MumuFriend::updateServerAddress(QString address)
{
	DatabaseManager::getInstance()->updateServerAddress(address);
}

QObject *MumuFriend::retornaPendentesModel()
{
	return DatabaseManager::getInstance()->retornaPendetesModel();
}
