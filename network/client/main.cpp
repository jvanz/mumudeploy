
#include <mumuclient.h>
#include <QCoreApplication>
#include <iostream>

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	MumuClient client(&app);
	return app.exec();
}
