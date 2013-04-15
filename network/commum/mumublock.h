#include "util.h"


class MumuBlock : public QByteArray
{

private:
	QByteArray md5;

public:
	MumuBlock(QByteArray);
	QByteArray getMd5();
	void setMd5(QByteArray);

};
