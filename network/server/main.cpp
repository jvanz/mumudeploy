
#include <QCoreApplication>
#include <iostream>
#include "mumuserver.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	MumuServer server("/home/vanz/server/teste.mp3",&app);
	return app.exec();
}
