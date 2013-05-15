#include <QThread>

class MumuThread : public QThread
{
	Q_OBJECT
public:
	void run();
};
