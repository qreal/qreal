#pragma once

#include <interpreterBase/blocksBase/common/waitBlock.h>
#include <interpreterBase/robotModel/robotParts/button.h>

namespace interpreterBase {
namespace blocksBase {
namespace common {

/// An interpreter`s implementation for the buttons wait block.
/// @todo Inherit it from WaitForSensorBlock?
class ROBOTS_INTERPRETER_BASE_EXPORT WaitForButtonBlock : public WaitBlock
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model, determines port by 'Button' property of a port.
	WaitForButtonBlock(robotModel::RobotModelInterface &robotModel);

private slots:
	/// Called when new data about button state is received.
	void responseSlot(int isPressed);

	void timerTimeout() override;

private:
	void run() override;

	robotModel::DeviceInfo device() const override;

	robotModel::robotParts::Button *mButton;  // Does not have ownership.
};

}
}
}
