#pragma once

#include <interpreterBase/blocksBase/common/waitForSensorBlock.h>

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class WaitForMotionBlock : public interpreterBase::blocksBase::common::WaitForSensorBlock
{
public:
	explicit WaitForMotionBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private slots:
	void responseSlot(int reading) override;

private:
	QString port() const override;
	interpreterBase::robotModel::DeviceInfo device() const override;
};

}
}
}
