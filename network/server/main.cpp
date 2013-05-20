
#include <QCoreApplication>
#include <iostream>
#include "../networkmanager.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	QDir homeApp("/home/vanz/server");
	MumuServer server(homeApp,6666,&app);
	return app.exec();
}
