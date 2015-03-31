#pragma once

#include <kitBase/blocksBase/common/waitBlock.h>

namespace kitBase {
namespace blocksBase {
namespace common {

/// A base for all blocks that wait for sensor.
class ROBOTS_KIT_BASE_EXPORT WaitForSensorBlock : public WaitBlock
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit WaitForSensorBlock(robotModel::RobotModelInterface &robotModel);

	void run() override;

	void stopActiveTimerInBlock() override;

protected slots:
	void timerTimeout() override;

protected:
	void stop() override;

	robotModel::PortInfo mPort;

private:
	/// Shall be reimplemented to do value check when new data from sensor is ready.
	virtual void responseSlot(int reading) = 0;
};

}
}
}
