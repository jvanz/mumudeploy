#include <QtDeclarative>
#include <QUrl>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QDeclarativeView view;
    view.setSource(QUrl("qml/Main.qml"));
    view.show();
    return app.exec();
}
