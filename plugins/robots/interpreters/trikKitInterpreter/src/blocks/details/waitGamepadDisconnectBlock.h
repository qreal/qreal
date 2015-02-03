#pragma once

#include <interpreterBase/blocksBase/common/waitForSensorBlock.h>

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class WaitGamepadDisconnectBlock : public interpreterBase::blocksBase::common::WaitForSensorBlock
{
public:
	explicit WaitGamepadDisconnectBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private slots:
	void responseSlot(int reading) override;

private:
	QString port() override;
	interpreterBase::robotModel::DeviceInfo device() const override;
};

}
}
}
