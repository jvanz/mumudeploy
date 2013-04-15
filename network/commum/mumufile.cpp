
#include "mumufile.h"

MumuFile::MumuFile(QString path) : QFile(path)
{
	this->getMd5();
	this->splitFile();
	
}

void MumuFile::splitFile(int blocksCount)
{
	if(this->open(QIODevice::ReadOnly)){
		QByteArray blockFile = this->readAll();
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
		this->close();
	}
}

QByteArray MumuFile::getMd5()
{
	if(this->md5.isEmpty()){
		if(this->open(QIODevice::ReadOnly)){
			this->md5 = Util::generateMd5(this->readAll());
			this->close();
		}
	}
	Util::logMessage("MD5 = " + QString(this->md5.toHex()));
	return this->md5;
}

QList<MumuBlock> MumuFile::getBlocks()
{
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
