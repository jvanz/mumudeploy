#include "filedescriptor.h"

FileDescriptor::FileDescriptor(QByteArray block)
{
	this->generateFileDescriptor(block);
}

QString FileDescriptor::getFileName()
{
	return this->fileName;
}

void FileDescriptor::setFileName(QString fileName)
{
	this->fileName = fileName;
}

QByteArray FileDescriptor::getMd5()
{
	return this->md5;
}

void FileDescriptor::setMd5(QByteArray md5)
{
	this->md5 = md5;
}

quint8 FileDescriptor::getTotalBlocksCount()
{
	return this->totalBlocksCount;
}

void FileDescriptor::setTotalBlocksCount(quint8 totalBlocks)
{
	this->totalBlocksCount = totalBlocks;
}

QByteArray FileDescriptor::getBlockFileDescriptor()
{
	QByteArray block;
	QDataStream out(&block,QIODevice::WriteOnly);
	out << this->getFileName() << this->getTotalBlocksCount() << this->getMd5().constData();
	return block;
}
	
void FileDescriptor::generateFileDescriptor(QByteArray block)
{
	QDataStream in(&block,QIODevice::ReadOnly);
	in >> this->fileName >> this->totalBlocksCount >> this->md5;
}
