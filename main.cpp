#include <QtDeclarative>
#include <QUrl>
#include "controller/engine.h"
#include "client/mumuclient.h"
#include "controller/mumuthreadsend.h"
#include "controller/mumuthreadreceive.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QStringList args = app.arguments();

	if (args.count() > 1) {
		if (args[1] != "--headless") {
			qDebug() << "Expected --headless, received " << args[1] << ". Aborting!";
			return 1;
		}
		MumuClient client("/home/marcos/grafico.diff", QHostAddress::LocalHost, 6666, NULL);
		return app.exec();
	}

	MumuThreadSend threadSend;
	MumuThreadReceive threadReceive;
	threadSend.start();
	threadReceive.start();
	qDebug() << "Hello from gui thread " << app.thread()->currentThreadId();
	threadSend.wait();
	threadReceive.wait();

	QDeclarativeView view;

	Engine::start();

	view.setSource(QUrl("qml/Main.qml"));
	view.show();
	return app.exec();
}
