
#include <QTcpServer>
#include <QFile>

class MumuServer : public QTcpServer
{
	Q_OBJECT

private:
	QFile * file;

	void openFile();

public:
	MumuServer(QObject * parent); 

protected:
	void incomingConnection(int);

private slots:
	
	void clientConnecting();
};

