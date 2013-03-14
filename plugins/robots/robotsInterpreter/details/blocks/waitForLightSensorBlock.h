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

class WaitForLightSensorBlock : public Block
{
	Q_OBJECT

public:
	WaitForLightSensorBlock(RobotModel const * const robotModel);
	virtual void run();

	virtual QList<SensorPortPair> usedSensors() const;

	virtual void stopActiveTimerInBlock();

private slots:
	void responseSlot(int reading);
	void failureSlot();
	void timerTimeout();

private:
	static const int maxValue = 1023;

	inputPort::InputPortEnum mPort;
	RobotModel const * const mRobotModel;
	QTimer mActiveWaitingTimer;

	void stop();
};
}
}
}
}
}
