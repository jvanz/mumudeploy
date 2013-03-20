#include "databasemanager.h"
#include <QDir>
#include <QSqlQuery>

bool DatabaseManager::openDB()
{
	db = QSqlDatabase::addDatabase("QSQLITE");

	QString path(QDir::home().path());
	path.append(QDir::separator()).append("mumuDB.sqlite3");
	path = QDir::toNativeSeparators(path);

	db.setDatabaseName(path);

	return db.open();
}

void DatabaseManager::closeDB()
{
	db.close();
}

bool DatabaseManager::insertNewProcess(QString ip, QString path)
{
	QSqlQuery query;
	bool ret = true;

	if (!openDB()) {
		if (!query.exec(QString("INSERT INTO PROCESSES VALUES(%2, %2, 'N')").arg(ip).arg(path)))
			ret = false;

		closeDB();
	}


	return ret;
}
