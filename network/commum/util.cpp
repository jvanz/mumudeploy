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


void Util::sendBytesTo(char * data, QTcpSocket * socket)
{
	QByteArray  * tmpBlock = new QByteArray();
	QDataStream out(tmpBlock,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << STX;
	Util::logMessage("Data size = " + QString::number(sizeof(*data)/sizeof(char)));
	while(*data){
		out << *data;
		data++;
	}
	out << ETX;
	int bytes = socket->write(tmpBlock->constData(), sizeof(tmpBlock->constData()));
	Util::logMessage(QString::number(bytes));
}

void Util::sendMsgTo(char msg, QTcpSocket * socket)
{
	Util::sendBytesTo(&msg, socket);
	
}

QByteArray Util::processData(QTcpSocket * socket)
{
	Util::logMessage("Socket has data to process.");
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_4_3);

	forever{
		if(socket->bytesAvailable() > 0){
			Util::logMessage("Bytes Available = " + QString::number(socket->bytesAvailable()));
		}
		char * byte;
		in.readRawData(byte, sizeof(char));
		if(*byte == STX){ //STX - START OF TEXT ( FIRST BYTE )
			Util::logMessage("Achou STX");
			continue;
		}
		if(*byte == ETX){ // ETX - END OF TEXT ( LAST BYTE )
			Util::logMessage("Achou ETX");
			break;
		}
		out << byte;
	}
	return block;
}

