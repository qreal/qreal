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

class WaitForColorIntensityBlock : public Block
{
	Q_OBJECT

public:
	WaitForColorIntensityBlock(RobotModel const * const robotModel);
	virtual void run();

	virtual QList<SensorPortPair> usedSensors() const;

	virtual void stopActiveTimerInBlock();

private slots:
	void responseSlot(int reading);
	void failureSlot();
	void timerTimeout();

private:
	RobotModel const * const mRobotModel;
	QTimer mActiveWaitingTimer;
	inputPort::InputPortEnum mPort;

	void stop();
};

}
}
}
}
}
