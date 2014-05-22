#pragma once

#include "waitForSensorBlock.h"

namespace interpreterBase {
namespace blocksBase {
namespace common {

class ROBOTS_INTERPRETER_BASE_EXPORT WaitForSonarDistanceBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	WaitForSonarDistanceBlock(interpreterBase::robotModel::RobotModelInterface &robotModel
			, interpreterBase::robotModel::DeviceInfo const &device);
	~WaitForSonarDistanceBlock() override;

protected slots:
	void responseSlot(int reading) override;

protected:
	interpreterBase::robotModel::DeviceInfo device() const override;

private:
	interpreterBase::robotModel::DeviceInfo const mDevice;
};

}
}
}
