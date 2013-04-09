#ifndef __MUMUFRIEND_H__

#include <QObject>
#include "server/mumuserver.h"

class MumuFriend : public QObject
{
	Q_OBJECT
public:
	MumuFriend(QObject *parent = 0);
	Q_INVOKABLE void sendFile(QString path);
};
#endif //__MUMUFRIEND_H__
