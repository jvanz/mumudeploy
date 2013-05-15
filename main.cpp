#include <QtDeclarative>
#include <QUrl>
#include "controller/engine.h"
#include "client/mumuclient.h"
#include "controller/mumuthread.h"

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

	MumuThread thread;
	thread.start();
	qDebug() << "Hello from gui thread " << app.thread()->currentThreadId();
	thread.wait();

	QDeclarativeView view;

	Engine::start();

	view.setSource(QUrl("qml/Main.qml"));
	view.show();
	return app.exec();
}
