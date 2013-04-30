#include "mumufriend.h"
#include "databasemanager.h"

MumuFriend::MumuFriend(QObject *parent)
	: QObject(parent)
{
}

bool MumuFriend::sendFile(QString ip, QString path)
{
	this->server = NetworkManager::getInstance(path, 6666, this->parent());
	this->server->sendFile(path);

	return DatabaseManager::getInstance()->insertNewProcess(ip, path, "S", this->server->getNumberOfParts());
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
