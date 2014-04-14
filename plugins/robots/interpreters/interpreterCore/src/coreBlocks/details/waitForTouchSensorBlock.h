#pragma once

#include "waitForSensorBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForTouchSensorBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForTouchSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForTouchSensorBlock() override;

protected slots:
	void responseSlot(int reading) override;

protected:
	interpreterBase::robotModel::DeviceInfo device() const override;
};

}
}
}
