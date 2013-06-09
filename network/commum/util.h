#include <QByteArray>
#include <QCryptographicHash>
#include <QString>
#include <QTcpSocket>

#include "../../controller/filehandle.h"

#define US  quint16(17) // unitu separator char. Used to separate field in the blocks
#define SOH quint16(1) // start connection
#define STX quint16(2) // begin block
#define ETX quint16(3) // end block
#define ACK quint16(6) // positive
#define NAK quint16(21) // negative
#define ENQ quint16(5) // request file
#define EOT quint16(23) // end of transmition

class Util
{

public:
	static QByteArray generateMd5(QByteArray);	
	static void logMessage(QString);	
	static void sendBytesTo(QByteArray, QTcpSocket*);
	static void sendMsgTo(quint16, QTcpSocket*);
	static QByteArray processData(QTcpSocket*);
	static quint16 processMsg(QByteArray);
	static QByteArray getBlockFile(QString);
	static bool saveBlockLikeFile(QDir,QByteArray, QString);
	static QByteArray loadFileBlock(QDir dir,QString fileName, int blockNumber);
};
