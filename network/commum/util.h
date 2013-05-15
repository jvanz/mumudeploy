#include <QByteArray>
#include <QCryptographicHash>
#include <QString>

#define SOH 1 // start connection
#define STX 2 // begin block
#define ETX 3 // end block
#define ACK 6 // positive
#define NAK 21 // negative
#define ENQ 5 // request file
#define ETB 23 // end of transmition


class Util
{

public:
	static QByteArray generateMd5(QByteArray);	
	static void logMessage(QString);	

};
