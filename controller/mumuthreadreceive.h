#include <QThread>

class MumuThreadReceive : public QThread
{
	Q_OBJECT
public:
	void run();
};
