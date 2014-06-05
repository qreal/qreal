#pragma once

#include "waitForSensorBlock.h"

namespace interpreterBase {
namespace blocksBase {
namespace common {

/// An interpreter`s implementation for the accelerometer`s wait block.
class ROBOTS_INTERPRETER_BASE_EXPORT WaitForAccelerometerSensorBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForAccelerometerSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForAccelerometerSensorBlock() override;

protected slots:
	void responseSlot(int reading) override;

protected:
	interpreterBase::robotModel::DeviceInfo device() const override;
};

}
}
}
