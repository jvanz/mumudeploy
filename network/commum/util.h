#include <QByteArray>
#include <QCryptographicHash>
#include <QString>

class Util
{

public:
	static QByteArray generateMd5(QByteArray);	
	static void logMessage(QString);	

};
