#include "databasemanager.h"
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>

DatabaseManager::DatabaseManager()
	: db(QSqlDatabase::addDatabase("QSQLITE"))
{
	db.setDatabaseName("mumuDB.db");
}

bool DatabaseManager::openDB()
{
	return db.open();
}

void DatabaseManager::closeDB()
{
	db.close();
}

bool DatabaseManager::insertNewProcess(QString ip, QString path)
{
	bool ret = true;

	if (openDB()) {
		QSqlQuery query(db);

		query.prepare("INSERT INTO PROCESSES(IP, FILE_PATH, SENT) VALUES (:IP, :PATH, 'N')");
		query.bindValue(":IP", ip);
		query.bindValue(":PATH", path);

		if (!query.exec()) {
			qDebug() << "Erro ao inserir registro!!";
			qDebug() << query.lastError().text();
			ret = false;
		}

		closeDB();
	}
	return ret;
}
