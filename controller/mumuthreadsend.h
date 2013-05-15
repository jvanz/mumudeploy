#include <QThread>

class MumuThreadSend : public QThread
{
	Q_OBJECT
public:
	void run();
};
