#pragma once

#include <interpreterBase/blocksBase/common/waitForSensorBlock.h>

#include "robotModel/parts/trikGamepadConnectionIndicator.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class WaitGamepadConnectBlock
		: public interpreterBase::blocksBase::common::WaitForSensorBlock
{
public:
	explicit WaitGamepadConnectBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private slots:
	void responseSlot(int reading) override;

private:
	QString port() override;
	interpreterBase::robotModel::DeviceInfo device() const override;
};

}
}
}
