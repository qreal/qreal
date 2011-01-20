#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "block.h"
#include "../robotParts/touchSensor.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class WaitForTouchSensorBlock : public Block
{
	Q_OBJECT

public:
	WaitForTouchSensorBlock(robotParts::TouchSensor &touchSensor);
	virtual void run();

private slots:
	void response(int reading);
	void timerTimeout();

private:
	robotParts::TouchSensor &mTouchSensor;
	QTimer mActiveWaitingTimer;
};

}
}
}
}
}
