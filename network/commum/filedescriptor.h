
#include <QString>
#include <QByteArray>
#include <QObject>

class FileDescriptor
{

private:
	QString fileName;
	QByteArray md5;
	quint8 totalBlocksCount;

public:
	FileDescriptor(QString file, QByteArray fileMd5, quint8 totalBlocks) : fileName(file), md5(fileMd5), totalBlocksCount(totalBlocks){};
	QString getFileName();
	void setFileName(QString);
	QByteArray getMd5();
	void setMd5(QByteArray);
	quint8 getTotalBlocksCount();
	void setTotalBlocksCount(quint8);

};
