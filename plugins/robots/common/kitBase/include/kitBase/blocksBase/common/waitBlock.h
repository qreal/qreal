#pragma once

#include <kitBase/blocksBase/robotsBlock.h>

namespace kitBase {
namespace blocksBase {
namespace common {

/// A base for all blocks that wait for sensor or button or something other.
class ROBOTS_KIT_BASE_EXPORT WaitBlock : public RobotsBlock
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit WaitBlock(robotModel::RobotModelInterface &robotModel);
	~WaitBlock() override;

	void setFailedStatus() override;

	QMap<robotModel::PortInfo, robotModel::DeviceInfo> usedDevices() override;

protected slots:
	/// Shall be called when something goes wrong and there will be no response from real device.
	void failureSlot();

protected:
	/// Stops waiting timer.
	/// @todo Shall not be virtual.
	virtual void stopActiveTimerInBlock();

	/// Checks that current sensor reading greater, less or so on than target value and stops waiting if reading is ok.
	void processResponce(int reading, int targetValue);

	/// Stops waiting and transfers control to a next block.
	virtual void stop();

	/// Implementation must return the port of the the device used in the block.
	/// Default implementation works with the port specified in the 'Port' property of the block.
	virtual QString port();

	/// Implementation must return the type of the waited device or DeviceInfo() if no such one.
	virtual robotModel::DeviceInfo device() const = 0;

	/// @todo Make it private.
	robotModel::RobotModelInterface &mRobotModel;
	QTimer mActiveWaitingTimer;

private slots:
	/// Shall be redefined in subclasses to request for a sensor or button to read its value.
	virtual void timerTimeout() = 0;
};

}
}
}
