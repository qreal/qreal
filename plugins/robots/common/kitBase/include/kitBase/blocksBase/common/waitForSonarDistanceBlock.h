#pragma once

#include "waitForSensorBlock.h"

namespace kitBase {
namespace blocksBase {
namespace common {

/// An interpreter`s implementation for "wait for sonar sensor" block.
class ROBOTS_KIT_BASE_EXPORT WaitForSonarDistanceBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	WaitForSonarDistanceBlock(robotModel::RobotModelInterface &robotModel, const robotModel::DeviceInfo &device);

protected slots:
	void responseSlot(int reading) override;

protected:
	kitBase::robotModel::DeviceInfo device() const override;

private:
	const kitBase::robotModel::DeviceInfo mDevice;
};

}
}
}
