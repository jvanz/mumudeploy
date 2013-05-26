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
	
/*	Util::logMessage("Socket has data to process.");
	QByteArray array;

	while(!array.contains(ETX)) {
		array += socket->readAll();
	}
	Util::logMessage("Array size = " + QString::number(array.size()));
	QDataStream in(&array, QIODevice::ReadOnly);
	QByteArray message;
	in >> message;
	Util::logMessage("Message size = " + QString::number(message.size()));
	return message;
*/
}

quint16 Util::processMsg(QByteArray block)
{
	QDataStream in(&block, QIODevice::ReadOnly);
	quint16 byte;
	in >> byte;
	Util::logMessage("MSG = " + QString::number(byte));
	return byte;

}
