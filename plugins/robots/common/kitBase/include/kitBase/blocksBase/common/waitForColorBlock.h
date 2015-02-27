#pragma once

#include <kitBase/blocksBase/common/waitForColorSensorBlockBase.h>

namespace kitBase {
namespace blocksBase {
namespace common {

/// An interpreter`s implementation for "wait for color sensor" block.
class ROBOTS_KIT_BASE_EXPORT WaitForColorBlock : public WaitForColorSensorBlockBase
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit WaitForColorBlock(robotModel::RobotModelInterface &robotModel);

protected slots:
	void responseSlot(int reading) override;

private:
	robotModel::DeviceInfo device() const override;
};

}
}
}
