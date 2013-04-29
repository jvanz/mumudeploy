
#include <mumuclient.h>
#include <QCoreApplication>
#include <iostream>

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	MumuClient client("/home/vanz/client/teste.mp3",QHostAddress::LocalHost,6666,&app);
	return app.exec();
}
