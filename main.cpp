#include <QtDeclarative>
#include <QUrl>
#include "controller/engine.h"
#include "client/mumuclient.h"
#include "server/mumuserver.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QStringList args = app.arguments();

	if (args.count() > 1) {
		if (args[1] == "-client" | args[1] == "-server") {
			if( args.count() >= 3){
				if(args[2] == "-port" & args[3].toInt() != 0){
					int port = args[3].toInt(); 
					if(args[1] == "-client"){
						MumuClient client(QHostAddress::LocalHost, port , &app);
					}else{
						MumuServer server(port, &app);
					}
				}else{
					qDebug() << "Expected [-client|-server] -port <port>. Aborting!";
					return 1;
				}
			QDeclarativeView view;
			Engine::start();
			view.setSource(QUrl("qml/Main.qml"));
			view.show();
			return app.exec();
			}else{
				qDebug() << "Expected [-client|-server] -port <port>.  Aborting!";
				return 1;

			}	
		}else{
			qDebug() << "Expected -client or -server, received " << args[1] << ". Aborting!";
			return 1;
		}
	}
	qDebug() << "Expected [-client|-server] -port <port>  Aborting!";
	return 1;

}
