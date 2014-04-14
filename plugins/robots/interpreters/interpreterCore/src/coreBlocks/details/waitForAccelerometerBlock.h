#pragma once

#include "waitForSensorBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForAccelerometerSensorBlock : public WaitForSensorBlock
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
