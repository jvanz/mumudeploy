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

		verifyNewDatabase();

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

// if we're in a new database
void DatabaseManager::verifyNewDatabase()
{
	QSqlQuery query(db);
	query.exec("CREATE TABLE IF NOT EXISTS PROCESSES(IP TEXT(255), FILE_PATH TEXT(255), SENT TEXT(1));");
}

