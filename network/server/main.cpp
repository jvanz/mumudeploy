
#include <QCoreApplication>
#include <iostream>
#include "../networkmanager.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	QDir homeApp = QDir::toNativeSeparators(FileHandle::getPublicUserHome().path());
	MumuServer server(6666,&app);
	return app.exec();
}
