#include <QtDeclarative>
#include <QSqlDatabase>
#include "qmlinterface.h"

class DatabaseManager : public QObject
{
	Q_OBJECT
public:
	DatabaseManager();

	bool openDB();
	void closeDB();

public slots:
	Q_INVOKABLE bool insertNewProcess(QString ip, QString path);
	Q_INVOKABLE QObject *retornaPendetesModel();

private:
	QSqlDatabase db;
	void verifyNewDatabase();
	bool alreadySending(QString path, QString ip);
	QMLInterface model;
};
