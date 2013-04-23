#include <QtDeclarative>
#include <QFile>

class FileHandle : public QObject
{
	Q_OBJECT

public:
	FileHandle() : QObject(0) {}
	static QString getUserHome();
	
public slots:
	Q_INVOKABLE static bool verifyPath(QString path);
	Q_INVOKABLE static bool verifyFile(QString fileName);
	Q_INVOKABLE static bool verifyDir(QString dirName);
};
