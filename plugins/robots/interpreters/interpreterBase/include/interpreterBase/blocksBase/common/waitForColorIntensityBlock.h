#pragma once

#include "waitForColorSensorBlockBase.h"

namespace interpreterBase {
namespace blocksBase {
namespace common {

/// An interpreter`s implementation for "wait for color intensity" block.
class ROBOTS_INTERPRETER_BASE_EXPORT WaitForColorIntensityBlock : public WaitForColorSensorBlockBase
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit WaitForColorIntensityBlock(robotModel::RobotModelInterface &robotModel);

protected slots:
	void responseSlot(int reading) override;
};

}
}
}
