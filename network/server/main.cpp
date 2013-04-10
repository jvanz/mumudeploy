
#include <QCoreApplication>
#include <iostream>
#include "../networkmanager.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	NetworkManager::getInstance("/home/vanz/server/file/teste.mp3",&app);
	return app.exec();
}
