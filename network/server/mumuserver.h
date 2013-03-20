
#include <QTcpServer>

class MumuServer : public QTcpServer
{
	Q_OBJECT

public:
	MumuServer(QObject * parent); 

private slots:
	
	void clientConnecting();
};

