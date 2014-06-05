#pragma once

#include "waitForColorSensorBlockBase.h"

namespace interpreterBase {
namespace blocksBase {
namespace common {

/// An interpreter`s implementation for the color sensor recognition wait block.
class ROBOTS_INTERPRETER_BASE_EXPORT WaitForColorBlock : public WaitForColorSensorBlockBase
{
	Q_OBJECT

public:
	explicit WaitForColorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForColorBlock() override;

protected slots:
	void responseSlot(int reading) override;
};

}
}
}
