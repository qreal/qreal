#pragma once

#include <QtCore/QTimer>

#include <utils/abstractTimer.h>

namespace utils {

class RealTimer : public AbstractTimer
{
public:
	RealTimer();

	virtual void start(int ms);

private:
	QTimer mTimer;
};

}
