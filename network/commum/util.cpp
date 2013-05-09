#include "util.h"

#include <QByteArray>
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
