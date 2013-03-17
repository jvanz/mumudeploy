#include <QtDeclarative>
#include <QUrl>
#include "engine.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QDeclarativeView view;

    Engine::start();

    view.setSource(QUrl("qml/Main.qml"));
    view.show();
    return app.exec();
}
