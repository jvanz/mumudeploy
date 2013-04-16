
#include <QList>
#include <QFile>
#include <QByteArray>
#include <QObject>

#include "mumublock.h"
#include "filedescriptor.h"

class MumuFile : QObject
{
	Q_OBJECT
	Q_PROPERTY(QByteArray md5 READ getMd5 WRITE setMd5)
	Q_PROPERTY(QList<MumuBlock> blocks READ getBlocks WRITE setBlocks)

private:
	QList<MumuBlock> blocks;
	QByteArray md5;
	QFile * file;

public:
	MumuFile(QString);
	void splitFile(int blocksCount = 3); 
	QByteArray getMd5();	
	void setMd5(QByteArray);
	QList<MumuBlock> getBlocks();
	void setBlocks(QList<MumuBlock>);
	MumuBlock getBlock(int);
	void getMumuDescription();
	FileDescriptor getFileDescriptor();
	QFile* getFile();
	QString fileName();
	bool exists();
};
