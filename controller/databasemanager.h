#include <QtDeclarative>
#include <QSqlDatabase>
#include "qmlinterface.h"

class DatabaseManager : public QObject
{
	Q_OBJECT
public:
	DatabaseManager();
	~DatabaseManager();

public slots:
	Q_INVOKABLE bool insertNewProcess(QString ip, QString path, QString direction);
	Q_INVOKABLE QObject *retornaPendetesModel();
	Q_INVOKABLE void updateDestDir(QString path);
	Q_INVOKABLE QString getDestinationDir();
	Q_INVOKABLE void updateServerAddress(QString path);
	Q_INVOKABLE QString getServerAddress();

private:
	QSqlDatabase db;
	void verifyNewDatabase();
	bool alreadySending(QString path, QString ip);
	QMLInterface model;
};
