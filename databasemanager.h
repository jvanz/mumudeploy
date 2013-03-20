#include <QSqlDatabase>

class DatabaseManager : public QObject
{
	Q_OBJECT;
public:
	DatabaseManager(QObject *parent = 0);
	~DatabaseManager();

	bool openDB();
	void cloneDB();

private:
	QSqlDatabase db;
};
