#pragma once

#include "waitForSensorBlock.h"

namespace interpreterBase {
namespace blocksBase {
namespace common {

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
