
#include <mumuclient.h>
#include <QCoreApplication>
#include <iostream>

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	std::cout<<"criou app"<<std::endl;
	MumuClient client(&app);
	std::cout<<"criou client"<<std::endl;
	return app.exec();
}
