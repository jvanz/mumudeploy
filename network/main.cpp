#include <QApplication>
#include <QtCore>
#include <QString>
#include <iostream>

#include <stdlib.h>

#include "server.h"
#include "client.h"

/**
 * This source is used to test the application.
 */

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QString mode(argv[1]);
	if(mode == "server"){ //server
		Server server;
		server.start();
		qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
		while(server.isRunning());
	}else{ //client
		Client client;		
		client.run();
	}
}
