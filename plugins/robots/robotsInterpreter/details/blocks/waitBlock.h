#pragma once

#include "block.h"

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace details
{
namespace blocks
{

class WaitBlock : public Block
{
	Q_OBJECT

public:
	explicit WaitBlock(RobotModel * const robotModel);
	virtual ~WaitBlock() {}

	virtual void setFailedStatus();
	virtual void stopActiveTimerInBlock();

protected slots:
	virtual void failureSlot();
	virtual void timerTimeout() = 0;

protected:
	void processResponce(int reading, int targetValue);
	virtual void stop();

	RobotModel * const mRobotModel;
	QTimer mActiveWaitingTimer;
};

}
}
}
}
}
