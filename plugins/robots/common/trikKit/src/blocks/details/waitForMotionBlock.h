#pragma once

#include <kitBase/blocksBase/common/waitForSensorBlock.h>

namespace trik {
namespace blocks {
namespace details {

class WaitForMotionBlock : public kitBase::blocksBase::common::WaitForSensorBlock
{
public:
	explicit WaitForMotionBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private slots:
	void responseSlot(int reading) override;

private:
	QString port();
	kitBase::robotModel::DeviceInfo device() const override;
};

}
}
}
