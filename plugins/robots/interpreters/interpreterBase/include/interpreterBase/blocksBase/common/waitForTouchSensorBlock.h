#pragma once

#include "waitForSensorBlock.h"

namespace interpreterBase {
namespace blocksBase {
namespace common {

/// An interpreter`s implementation for "wait for touch sensor" block.
class ROBOTS_INTERPRETER_BASE_EXPORT WaitForTouchSensorBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForTouchSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

protected slots:
	void responseSlot(int reading) override;

protected:
	interpreterBase::robotModel::DeviceInfo device() const override;
};

}
}
}
