#pragma once

#include <interpreterBase/blocksBase/common/waitBlock.h>

#include "robotModel/parts/trikGamepadButton.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

/// Implementation of 'TrikWaitGamepadButton' for interpreter, uses robot telemetry to wait for button press
/// on a computer.
class WaitGamepadButtonBlock : public interpreterBase::blocksBase::common::WaitBlock
{
public:
	explicit WaitGamepadButtonBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private slots:
	/// Called when new data about button state is received.
	void responseSlot(int isPressed);

	void timerTimeout() override;

private:
	void run() override;

	interpreterBase::robotModel::DeviceInfo device() const override;

	const QString mPort;
	robotModel::parts::TrikGamepadButton *mButton;  // Does not have ownership.
};

}
}
}
