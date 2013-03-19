#include "DatabaseManager.h"
#include <QDir>

bool DatabaseManager::openDB()
{
	db = QSqlDatabase::addDatabase("QSQLITE");

	QString path(QDir::home().path());
	path.append(QDir::separator()).append("mumuDB.sqlite");
	path = QDir::toNativeSeparators(path);

	db.setDatabaseName(path);

	return db.open();
}
