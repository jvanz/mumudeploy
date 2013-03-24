
#include <QCoreApplication>
#include <iostream>
#include "mumuserver.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	MumuServer server(&app);
	return app.exec();
}
