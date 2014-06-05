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
	explicit WaitForLightSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForLightSensorBlock() override;

protected slots:
	void responseSlot(int reading) override;

protected:
	interpreterBase::robotModel::DeviceInfo device() const override;
};
}
}
}
