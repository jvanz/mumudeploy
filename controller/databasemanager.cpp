#include "databasemanager.h"
#include "filehandle.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>

DatabaseManager *DatabaseManager::instance = 0;

DatabaseManager *DatabaseManager::getInstance()
{
	if (!instance)
		instance = new DatabaseManager();
	return instance;
}

DatabaseManager::DatabaseManager()
	: db(QSqlDatabase::addDatabase("QSQLITE"))
{
	db.setDatabaseName(QDir::toNativeSeparators(FileHandle::getUserHome() + "/mumuDB.db"));
	db.open();

	verifyNewDatabase();
}

DatabaseManager::~DatabaseManager()
{
	db.close();
}

void DatabaseManager::updateSentReceive(QString ip, QString file)
{
	QSqlQuery query(db);
	query.prepare("UPDATE PROCESSES SET SENT_PIECES = SENT_PIECES + 1 WHERE IP = :IP AND FILE_PATH = :FILE");
	query.bindValue(":IP", ip);
	query.bindValue(":FILE", file);
	query.exec();
}

bool DatabaseManager::insertNewProcess(QString ip, QString path, QString direction, int parts)
{
	bool ret = true;

	// verify if the same file is been send
	if (alreadySending(path, ip)) {
		qDebug() << "File is already sending";
		return false;
	}

	QFileInfo file(path);

	QSqlQuery query(db);
	query.prepare("INSERT INTO PROCESSES(IP, FILE_PATH, SENT, TOTAL_PIECES, SENT_PIECES, DIRECTION) "
			"VALUES (:IP, :PATH, 'N', :PARTS, 0, :DIRECTION)");
	query.bindValue(":IP", ip);
	query.bindValue(":PATH", file.fileName());
	query.bindValue(":DIRECTION", direction);
	query.bindValue(":PARTS", parts);

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
	
	QSqlQuery query(db);
	query.prepare("SELECT FILE_PATH "
			"FROM PROCESSES "
			"WHERE IP = :IP "
			"AND FILE_PATH = :FILE "
			"AND SENT = 'N' ");
	query.bindValue(":IP", ip);
	query.bindValue(":FILE", file.fileName());

	query.exec();
	if (query.next())
		return true;
	return false;
}

void DatabaseManager::updateSentFlag(QString ip, QString path)
{
	QSqlQuery query(db);
	query.prepare("UPDATE PROCESSES SET SENT = :SENT WHERE FILE_PATH = :PATH AND IP = :IP");
	query.bindValue(":FILE_PATH", path);
	query.bindValue(":IP", ip);
	query.bindValue(":SENT", "S");

	query.exec();
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
						"TOTAL_PIECES INTEGER, SENT_PIECES INTEGER, DIRECTION TEXT(1))");
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
			", TOTAL_PIECES "
			", SENT_PIECES "
			", DIRECTION "
			", IP "
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

QSqlQuery DatabaseManager::returnOpenProcesses()
{
	QSqlQuery query(db);
	query.prepare("SELECT FILE_PATH "
			", TOTAL_PIECES "
			", SENT_PIECES "
			", DIRECTION "
			", IP "
			"FROM PROCESSES "
		       "WHERE SENT = 'N'");
	query.exec();
	return query;
}

int DatabaseManager::nextPiece(QString file, QString ip)
{
	QSqlQuery query(db);

	query.prepare("SELECT SENT_PIECES FROM PROCESSES WHERE FILE_PATH = :FILE AND IP = :IP");
	query.bindValue(":FILE", file);
	query.bindValue(":IP", ip);

	if (query.exec() && query.next())
		return query.value(0).toInt() + 1;

	qWarning() << "File/IP: " << file << "/" << ip << " not found!!! ERROR: " << query.lastError().databaseText();
	return -1;
}
