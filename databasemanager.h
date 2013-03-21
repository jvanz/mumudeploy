#include <QtDeclarative>
#include <QSqlDatabase>

class DatabaseManager : public QObject
{
	Q_OBJECT
public:
	DatabaseManager();

	bool openDB();
	void closeDB();

public slots:
	Q_INVOKABLE bool insertNewProcess(QString ip, QString path);

private:
	QSqlDatabase db;
};
