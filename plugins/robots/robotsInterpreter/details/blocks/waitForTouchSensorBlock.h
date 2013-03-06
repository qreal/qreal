#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "block.h"
#include "../robotParts/robotModel.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class WaitForTouchSensorBlock : public Block
{
	Q_OBJECT

public:
	WaitForTouchSensorBlock(RobotModel const * const robotModel);
	virtual void run();

	virtual QList<SensorPortPair> usedSensors() const;

	virtual void stopActiveTimerInBlock();

private slots:
	void responseSlot(int reading);
	void failureSlot();
	void timerTimeout();

private:
	inputPort::InputPortEnum mPort;
	RobotModel const * const mRobotModel;
	QTimer mActiveWaitingTimer;
};

}
}
}
}
}
