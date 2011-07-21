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

class WaitForEncoderBlock : public Block
{
	Q_OBJECT

public:
	WaitForEncoderBlock(RobotModel *  const robotModel);
	virtual void run();

	virtual void stopActiveTimerInBlock();

private slots:
	void responseSlot(int reading);
	void failureSlot();
	void timerTimeout();

private:
	robotParts::EncoderSensor * mEncoderSensor;  // Doesn't have ownership
	RobotModel * const mRobotModel;
	QTimer mActiveWaitingTimer;

	void stop();
};

}
}
}
}
}
