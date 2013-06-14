
#include <mumuclient.h>
#include <QCoreApplication>
#include <iostream>

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	MumuClient client("/home/vanz/client/teste.mp3", QHostAddress("192.168.1.5") ,6666,&app);
	return app.exec();
}
