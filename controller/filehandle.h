#include <QtDeclarative>
#include <QFile>

class FileHandle : public QObject
{
	Q_OBJECT

public:
	FileHandle() : QObject(0) {}
	
public slots:
	Q_INVOKABLE bool verifyPath(QString path);
	Q_INVOKABLE bool verifyFile(QString fileName);
	Q_INVOKABLE bool verifyDir(QString dirName);
};
