
#include <QTcpSocket>
#include <QObject>

class MumuClient : QObject
{
	Q_OBJECT

public:
	MumuClient(QObject *parent);
private:
	bool connected;
	QTcpSocket tcpSocket;

	bool connectMumuServer();

private slots:
	void serverConnected();
};

