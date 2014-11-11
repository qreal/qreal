#pragma once

#include "waitForSensorBlock.h"

namespace interpreterBase {
namespace blocksBase {
namespace common {

/// An interpreter`s implementation for the light sensor`s wait block.
class ROBOTS_INTERPRETER_BASE_EXPORT WaitForLightSensorBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit WaitForLightSensorBlock(robotModel::RobotModelInterface &robotModel);

protected slots:
	void responseSlot(int reading) override;

protected:
	robotModel::DeviceInfo device() const override;
};
}
}
}
