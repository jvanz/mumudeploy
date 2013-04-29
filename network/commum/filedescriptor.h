#ifndef __FILEDESCRIPTOR_H__
#define __FILEDESCRIPTOR_H__

#include <QDataStream>
#include <QIODevice>
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
	FileDescriptor(QByteArray);
	FileDescriptor(){};
	QString getFileName();
	void setFileName(QString);
	QByteArray getMd5();
	void setMd5(QByteArray);
	quint8 getTotalBlocksCount();
	void setTotalBlocksCount(quint8);
	QByteArray getBlockFileDescriptor();
	void generateFileDescriptor(QByteArray);

};


#endif //__FILEDESCRIPTOR_H__
