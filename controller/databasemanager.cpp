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

		// verify if the same file is been send
		if (alreadySending(path, ip)) {
			qDebug() << "File is already sending";
			return false;
		}

		QFileInfo file(path);
		QString fileName = file.fileName();

		QSqlQuery query(db);

		query.prepare("INSERT INTO PROCESSES(IP, FILE_PATH, SENT) VALUES (:IP, :PATH, 'N')");
		query.bindValue(":IP", ip);
		query.bindValue(":PATH", fileName);

		if (!query.exec()) {
			qDebug() << "Erro ao inserir registro!!";
			qDebug() << query.lastError().text();
			ret = false;
		}

		closeDB();
	}
	return ret;
}

bool DatabaseManager::alreadySending(QString path, QString ip)
{
	QFileInfo file(path);
	QString fileName = file.fileName();
	
	QSqlQuery query(db);
	query.prepare("SELECT FILE_PATH "
			"FROM PROCESSES "
			"WHERE IP = :IP "
			"AND FILE_PATH = :FILE "
			"AND SENT = 'N' ");
	query.bindValue(":IP", ip);
	query.bindValue(":FILE", fileName);

	query.exec();
	if (query.next())
		return true;
	return false;
}

// if we're in a new database
void DatabaseManager::verifyNewDatabase()
{
	QSqlQuery query(db);
	query.exec("CREATE TABLE IF NOT EXISTS PROCESSES(IP TEXT(255), FILE_PATH TEXT(255), SENT TEXT(1));");
}

QObject *DatabaseManager::retornaPendetesModel()
{
	if (openDB()) {
		QSqlQuery query;
		query.prepare("SELECT FILE_PATH "
				"FROM PROCESSES "
			       "WHERE SENT = 'N'");
		model.exec(query);
		closeDB();
		return &model;
	}
	return &model;
}
