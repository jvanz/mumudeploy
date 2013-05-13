
#include <QDir>

#include "mumufile.h"

MumuFile::MumuFile(QString path)
{
	this->descriptor;
	this->file = new QFile(path);
	this->descriptor.setFileName(file->fileName());
	this->getMd5();
	this->splitFile();
}

MumuFile::MumuFile(QByteArray fdBlock)
{
	this->descriptor = FileDescriptor(fdBlock);	
}

MumuFile::MumuFile(FileDescriptor fd) : descriptor(fd)
{}

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
		this->descriptor.setTotalBlocksCount(this->blocks.size());
	}
}

QByteArray MumuFile::getMd5()
{
	if(this->descriptor.getMd5().isEmpty()){
		if(this->file->open(QIODevice::ReadOnly)){
			this->descriptor.setMd5(Util::generateMd5(this->file->readAll()));
			this->file->close();
		}
	}
	Util::logMessage("MD5 = " + QString(this->descriptor.getMd5().toHex()));
	return this->descriptor.getMd5();
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
	this->descriptor.setMd5(md5);
}
	
void MumuFile::setBlocks(QList<MumuBlock> blocks)
{
	this->blocks = blocks;
}


FileDescriptor MumuFile::getFileDescriptor()
{
	return this->descriptor;
}
	
QFile* MumuFile::getFile()
{
	return this->file;
}

QString MumuFile::fileName()
{
	return this->descriptor.getFileName();
}
	
bool MumuFile::exists()
{
	return this->getFile()->exists();
}

QByteArray MumuFile::compress(const QByteArray& data)
{
	return qCompress(data, 9);
}

QByteArray MumuFile::uncompress(const QByteArray& data)
{
	return qUncompress(data);
}

int MumuFile::getSize()
{
	return getBlocks().size();
}
