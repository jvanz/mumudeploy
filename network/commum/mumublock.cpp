#include "mumublock.h"

MumuBlock::MumuBlock(QByteArray data) : QByteArray(data)
{
	//Nothing to do
}

QByteArray MumuBlock::getMd5()
{
	if(md5.isEmpty()){
		//TODO
	}
	return md5;
}
	
void MumuBlock::setMd5(QByteArray md5)
{
	this->md5 = md5;
}
