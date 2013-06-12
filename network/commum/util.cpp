#include "util.h"

#include <QByteArray>
#include <QDataStream>
#include <iostream>



QByteArray Util::generateMd5(QByteArray block)
{
	QCryptographicHash* hash;
	QByteArray result = hash->hash(block,QCryptographicHash::Md5);
	return result;
}


void Util::logMessage(QString msg)
{
	std::cout << msg.toStdString() << std::endl;
}


void Util::sendBytesTo(QByteArray data, QTcpSocket * socket)
{
	QByteArray tmpBlock;
	QDataStream out(&tmpBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << qint64(0) << data;
	out.device()->seek(0);
	Util::logMessage("BLOCK SENT SIZE = " + QString::number(tmpBlock.size() - sizeof(qint64)));
	out << qint64(tmpBlock.size() - sizeof(qint64));
	socket->write(tmpBlock);
	Util::logMessage("Bytes sent. Block size = " + QString::number(tmpBlock.size()));
}

void Util::sendMsgTo(quint16 msg, QTcpSocket * socket)
{
	QByteArray block;
	QDataStream out(&block,QIODevice::WriteOnly);
	out << msg;
	Util::sendBytesTo(block, socket);
}

QByteArray Util::processData(QTcpSocket * socket)
{
	QDataStream in(socket);
	qint64 nextBlockSize = 0;
	Util::logMessage("Processing data... bytes = " + QString::number(socket->bytesAvailable()));
	forever{
		if(nextBlockSize == 0){
			if(socket->bytesAvailable() >= sizeof(qint64)){
				in >> nextBlockSize;	
				Util::logMessage("nextBlockSize = " + QString::number(nextBlockSize));
				Util::logMessage("Bytes Available = " + QString::number(socket->bytesAvailable()));
			}
		}
		Util::logMessage("Bytes Available = " + QString::number(socket->bytesAvailable()));
		if(socket->bytesAvailable() == nextBlockSize){
			break;
		}

	}
	QByteArray block;
	in >> block;
}

quint16 Util::processMsg(QByteArray block)
{
	QDataStream in(&block, QIODevice::ReadOnly);
	quint16 byte;
	in >> byte;
	Util::logMessage("MSG = " + QString::number(byte));
	return byte;

}
	
QByteArray Util::getBlockFile(QString fileName)
{
	QString path = FileHandle::getPublicUserHome().path() +"/file/" + fileName;
	Util::logMessage("Path = " + path);
	QFile file(path);
	if(file.open(QIODevice::ReadOnly)){
		return file.readAll();
	}
	return NULL;
}


bool Util::saveBlockLikeFile(QDir dir,QByteArray block, QString sufix)
{
	if(!dir.exists()){
		if(!dir.mkpath(dir.path())){
			Util::logMessage("não conseguiu criar a pasta");
			return false;
		}
	}
	QString path = dir.path() + "/block-" + sufix;
	Util::logMessage(path);
	QFile fileBlock(path);
	fileBlock.open(QIODevice::WriteOnly);
	QDataStream out(&fileBlock);
	int write = out.writeRawData(block.constData(),block.size());
	Util::logMessage("BYTES WRITEN = " + QString::number(write));
	fileBlock.close();
	return true;
}
	
QByteArray Util::loadFileBlock(QDir dir,QString fileName, int blockNumber)
{
	if(!dir.exists()){
		if(!dir.mkpath(dir.path())){
			return NULL;
		}
	}
	QString path = dir.path() + "/" + fileName + "/block-" + QString::number(blockNumber);
	Util::logMessage(path);
	QFile fileBlock(path);
	if(fileBlock.open(QIODevice::ReadOnly)){
		QByteArray block =  fileBlock.readAll();
		Util::logMessage("LOADFILEBLOCK = " + QString::number(block.size()));
		return block;
		
	}
	Util::logMessage("BLOCK DOES NOT OPEN");
	return NULL;
	
}
