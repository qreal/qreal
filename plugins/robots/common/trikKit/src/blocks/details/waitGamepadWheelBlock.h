#pragma once

#include <kitBase/blocksBase/common/waitForSensorBlock.h>

namespace trik {
namespace blocks {
namespace details {

class WaitGamepadWheelBlock : public kitBase::blocksBase::common::WaitForSensorBlock
{
public:
	explicit WaitGamepadWheelBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private slots:
	void responseSlot(int reading) override;

private:
	QString port() override;
	kitBase::robotModel::DeviceInfo device() const override;
};

}
}
}
