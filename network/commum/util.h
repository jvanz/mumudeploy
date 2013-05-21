#include <QByteArray>
#include <QCryptographicHash>
#include <QString>
#include <QTcpSocket>

#define US  17 // unitu separator char. Used to separate field in the blocks
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
	static void sendBytesTo(char*, QTcpSocket*);
	static void sendMsgTo(char, QTcpSocket*);
	static QByteArray processData(QTcpSocket*);

};
