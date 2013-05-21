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
	QDataStream out(&tmpBlock,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << data << ETX;
	int bytes = socket->write(tmpBlock);
	Util::logMessage("Bytes writen = " + QString::number(bytes));
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
	Util::logMessage("Socket has data to process.");
	QByteArray array;

	while(!array.contains(ETX)) {
		array += socket->readAll();
	}

	QDataStream in(&array, QIODevice::ReadOnly);
	QByteArray message;
	in >> message;
	return message;
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

