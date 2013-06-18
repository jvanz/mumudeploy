#include <QtDeclarative>
#include <QUrl>
#include "controller/engine.h"
#include "client/mumuclient.h"
#include "server/mumuserver.h"

/**
TODO - Melhoras essas validações. TA UMA ZONA!
*/

void PrintUsage()
{
 	qDebug() << "USAGE:";
	qDebug() << "mumudeploy [-client|-server] [-ip <ip>] -port <port>";
}

int ExtractArgs(QStringList * args, QString *ip, int * port)
{
	bool ok = false;
	if (args->size() >= 4) {

		if (args->at(1) == "-client" || args->at(1)== "-server" ) { // first arg ok

			if (args->at(1) == "-client" && args->size() >= 6) { // validate client args
				if (args->at(2) == "-ip" && args->at(4) == "-port") {
					if (args->at(3) != NULL && args->at(5) != NULL) {
						*ip = args->at(3);
						*port = args->at(5).toInt();
						ok = true;
					}
				}

			} else if (args->at(1) == "-server") { // validate server args
				if (args->at(2) == "-port" && args->at(3) != NULL) {
					*port = args->at(3).toInt();
					ok = true;
				}
			}
		}
	}

	if (!ok) {
		PrintUsage();
		return -1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QStringList args = app.arguments();
	QString ipServer;
	int port;
	MumuServer *server = 0;
	MumuClient *client = 0;
	
	if(ExtractArgs(&args, &ipServer, &port)){
		return -1;
	}

	if(args.at(1) == "-client"){
		client = new MumuClient(QHostAddress(ipServer), port,&app);
	}else if(args.at(1) == "-server"){
		server = new MumuServer(port, &app);
	}

	QDeclarativeView view;
	Engine::start();
	view.setSource(QUrl("qml/Main.qml"));
	view.show();
	app.exec();

	delete client;
	delete server;

	return 0;
}
