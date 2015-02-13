#pragma once

#include <interpreterBase/blocksBase/common/waitForColorSensorBlockBase.h>

namespace interpreterBase {
namespace blocksBase {
namespace common {

/// An interpreter`s implementation for "wait for color sensor" block.
class ROBOTS_INTERPRETER_BASE_EXPORT WaitForColorBlock : public WaitForColorSensorBlockBase
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
