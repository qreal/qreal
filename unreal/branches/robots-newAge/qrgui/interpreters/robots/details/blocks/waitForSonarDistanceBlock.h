#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "block.h"
#include "../robotModel.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class WaitForSonarDistanceBlock : public Block
{
	Q_OBJECT

public:
	WaitForSonarDistanceBlock(RobotModel const * const robotModel);
	virtual void run();

	virtual QList<SensorPortPair> usedSensors() const;

private slots:
	void response(int reading);
	void failure();
	void timerTimeout();

private:
	robotParts::SonarSensor *mSonarSensor;  // Doesn't have ownership
	RobotModel const * const mRobotModel;
	QTimer mActiveWaitingTimer;
};

}
}
}
}
}
