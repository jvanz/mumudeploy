#include <QtDeclarative>
#include <QSqlDatabase>

class DatabaseManager : public QObject
{
	Q_OBJECT
public:
	DatabaseManager() : QObject(0) {}

	bool openDB();
	void closeDB();

public slots:
	Q_INVOKABLE bool insertNewProcess(QString ip, QString path);

private:
	QSqlDatabase db;
};
