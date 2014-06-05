#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterBase {
namespace blocksBase {
namespace common {

/// A base for all blocks that wait for sensor or button or something other.
class ROBOTS_INTERPRETER_BASE_EXPORT WaitBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit WaitBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitBlock() override;

	void setFailedStatus() override;
	virtual void stopActiveTimerInBlock();

	QMap<robotModel::PortInfo, robotModel::DeviceInfo> usedDevices() const override;

protected slots:
	virtual void failureSlot();
	virtual void timerTimeout() = 0;

protected:
	/// Implementation must return the type of the waited device or DeviceInfo() if no such one.
	virtual robotModel::DeviceInfo device() const = 0;
	void processResponce(int reading, int targetValue);
	virtual void stop();

	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
	QTimer mActiveWaitingTimer;
};

}
}
}
