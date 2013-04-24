
#include <QCoreApplication>
#include <iostream>
#include "../networkmanager.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	QDir homeApp("/home/marcos/server");
	MumuServer server(homeApp,&app);
	return app.exec();
}
