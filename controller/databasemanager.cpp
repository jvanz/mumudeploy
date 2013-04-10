#include "databasemanager.h"
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>

DatabaseManager::DatabaseManager()
	: db(QSqlDatabase::addDatabase("QSQLITE"))
{
	db.setDatabaseName("mumuDB.db");
	db.open();
}

DatabaseManager::~DatabaseManager()
{
	db.close();
}

bool DatabaseManager::insertNewProcess(QString ip, QString path)
{
	bool ret = true;

	verifyNewDatabase();

	// verify if the same file is been send
	if (alreadySending(path, ip)) {
		qDebug() << "File is already sending";
		return false;
	}

	QFileInfo file(path);
	QString fileName = file.fileName();

	QSqlQuery query(db);

	query.prepare("INSERT INTO PROCESSES(IP, FILE_PATH, SENT, TOTAL_PIECES, SENT_PIECES) "
			"VALUES (:IP, :PATH, 'N', 0, 0)");
	query.bindValue(":IP", ip);
	query.bindValue(":PATH", fileName);

	if (!query.exec()) {
		qDebug() << "Erro ao inserir registro!!";
		qDebug() << query.lastError().text();
		ret = false;
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

void DatabaseManager::updateDestDir(QString path)
{
	QSqlQuery query(db);
	query.prepare("UPDATE CONFIGS SET DEST_PATH = :DIR_PATH");
	query.bindValue(":DIR_PATH", path);

	query.exec();
}

// if we're in a new database
void DatabaseManager::verifyNewDatabase()
{
	QSqlQuery query(db);
	query.exec("CREATE TABLE IF NOT EXISTS PROCESSES(IP TEXT(255), FILE_PATH TEXT(255), SENT TEXT(1), "
						"TOTAL_PIECES INTEGER, SENT_PIECES INTEGER)");
	query.exec("CREATE TABLE IF NOT EXISTS CONFIGS(DEST_PATH TEXT(255), SERVER_ADDR TEXT(255));");

	//just one register inside configs
	query.prepare("SELECT 1 FROM CONFIGS");
	query.exec();

	if (!query.next())
		query.exec("INSERT INTO CONFIGS(DEST_PATH, SERVER_ADDR) VALUES('', '');");
}

QObject *DatabaseManager::retornaPendetesModel()
{
	QSqlQuery query(db);
	query.prepare("SELECT FILE_PATH "
			"FROM PROCESSES "
		       "WHERE SENT = 'N'");
	model.exec(query);

	return &model;
}

QString DatabaseManager::getDestinationDir()
{
	QSqlQuery query(db);
	query.prepare("SELECT DEST_PATH FROM CONFIGS");
	query.exec();

	if (query.next())
		return query.value(0).toString();
	return "";
}

void DatabaseManager::updateServerAddress(QString address)
{
	QSqlQuery query(db);
	query.prepare("UPDATE CONFIGS SET SERVER_ADDR = :ADDRESS");
	query.bindValue(":ADDRESS", address);

	query.exec();
}

QString DatabaseManager::getServerAddress()
{
	QSqlQuery query(db);
	query.prepare("SELECT SERVER_ADDR FROM CONFIGS");
	query.exec();

	if (query.next())
		return query.value(0).toString();
	return "";
}
