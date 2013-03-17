#include <QtDeclarative>
#include <QFile>

class FileHandle : public QObject
{
	Q_OBJECT

public:
	FileHandle() : QObject(0) {}
	
public slots:
	Q_INVOKABLE void test();

private:
	QString fileName;
};
