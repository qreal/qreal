#pragma once

#include <interpreterBase/blocksBase/common/waitForSensorBlock.h>

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class WaitPadPressBlock : public interpreterBase::blocksBase::common::WaitForSensorBlock
{
public:
	explicit WaitPadPressBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private slots:
	void responseSlot(int reading) override;

private:
	QString port() override;
	interpreterBase::robotModel::DeviceInfo device() const override;
};

}
}
}
