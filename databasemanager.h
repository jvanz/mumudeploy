#include <QSqlDatabase>

class DatabaseManager : public QObject
{
public:
	DatabaseManager(QObject *parent = 0);
	~DatabaseManager();

	bool dbOpen();

private:
	QSqlDatabase db;
}
