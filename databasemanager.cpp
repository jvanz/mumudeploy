#include "databasemanager.h"
#include <QDir>

bool DatabaseManager::openDB()
{
	db = QSqlDatabase::addDatabase("QSQLITE");

	QString path(QDir::home().path());
	path.append(QDir::separator()).append("mumuDB.sqlite3");
	path = QDir::toNativeSeparators(path);

	db.setDatabaseName(path);

	return db.open();
}

void closeDB()
{
	db.close();
}
