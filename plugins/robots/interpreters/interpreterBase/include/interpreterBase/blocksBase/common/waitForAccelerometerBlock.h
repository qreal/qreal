#pragma once

#include "waitForSensorBlock.h"

namespace interpreterBase {
namespace blocksBase {
namespace common {

/// An interpreter`s implementation for "wait for accelerometer" block.
class ROBOTS_INTERPRETER_BASE_EXPORT WaitForAccelerometerSensorBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit WaitForAccelerometerSensorBlock(robotModel::RobotModelInterface &robotModel);

protected slots:
	void responseSlot(int reading) override;

protected:
	robotModel::DeviceInfo device() const override;
};

}
}
}
