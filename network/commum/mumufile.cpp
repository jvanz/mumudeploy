
#include <QDir>

#include "mumufile.h"

MumuFile::MumuFile(QString path)
{
	this->file = new QFile(QDir::toNativeSeparators(path));
	this->getMd5();
	this->splitFile();
}

void MumuFile::splitFile(int blocksCount)
{
	if(this->file->open(QIODevice::ReadOnly)){
		QByteArray blockFile = this->file->readAll();
		int blockSize = blockFile.size() / blocksCount;
		int totalBlocksBytes = 0;
		int lastPos = 0;
		while(this->blocks.count() < blocksCount){
			QByteArray b = blockFile.mid(lastPos, blockSize);
			MumuBlock block = b;
			totalBlocksBytes += block.size();
			this->blocks.append(block);
			lastPos = blockSize * blocks.size();
		}
		if(blockFile.size() != totalBlocksBytes){
			lastPos = blockSize * blocks.size();
			QByteArray b = blockFile.mid(lastPos, blockSize);
			MumuBlock block = b;
			totalBlocksBytes += block.size();
			this->blocks.append(block);
		}
		this->file->close();
	}
}

QByteArray MumuFile::getMd5()
{
	if(this->md5.isEmpty()){
		if(this->file->open(QIODevice::ReadOnly)){
			this->md5 = Util::generateMd5(this->file->readAll());
			this->file->close();
		}
	}
	Util::logMessage("MD5 = " + QString(this->md5.toHex()));
	return this->md5;
}

QList<MumuBlock> MumuFile::getBlocks()
{
	if(this->blocks.size() == 0){
		this->splitFile();
	}
	return this->blocks;
}

MumuBlock MumuFile::getBlock(int blockIndex)
{
	return this->blocks.at(blockIndex);
}

void MumuFile::setMd5(QByteArray md5)
{
	this->md5 = md5;
}
	
void MumuFile::setBlocks(QList<MumuBlock> blocks)
{
	this->blocks = blocks;
}


FileDescriptor MumuFile::getFileDescriptor()
{
	return FileDescriptor(this->file->fileName(), this->getMd5(), this->getBlocks().size());
}
	
QFile* MumuFile::getFile()
{
	return this->file;
}

QString MumuFile::fileName()
{
	return this->getFile()->fileName();
}
	
bool MumuFile::exists()
{
	return this->getFile()->exists();
}

QByteArray MumuFile::compress(const QByteArray& data)
{
	Util::logMessage("Antes compress = " + QString::number(((QByteArray)data).size()));
	Util::logMessage("Depois compress = " + QString::number(qCompress(data).size()));
	return qCompress(data);
}

QByteArray MumuFile::uncompress(const QByteArray& data)
{
	return qUncompress(data);
}

int MumuFile::getSize()
{
	return getBlocks().size();
}
