
#include <QCoreApplication>
#include <iostream>
#include "mumuserver.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	std::cout<<"criou app"<<std::endl;
	MumuServer server(&app);
	std::cout<<"criou server"<<std::endl;
	return app.exec();
}
