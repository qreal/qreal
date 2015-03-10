#pragma once

#include <kitBase/blocksBase/common/waitForSensorBlock.h>

#include "trikKit/robotModel/parts/trikGamepadConnectionIndicator.h"

namespace trik {
namespace blocks {
namespace details {

class WaitGamepadConnectBlock
		: public kitBase::blocksBase::common::WaitForSensorBlock
{
public:
	explicit WaitGamepadConnectBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private slots:
	void responseSlot(int reading) override;

private:
	QString port() override;
	kitBase::robotModel::DeviceInfo device() const override;
};

}
}
}
