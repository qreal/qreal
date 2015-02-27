#pragma once

#include <kitBase/blocksBase/common/waitBlock.h>

#include "trikKit/robotModel/parts/trikGamepadButton.h"

namespace trik {
namespace blocks {
namespace details {

/// Implementation of 'TrikWaitGamepadButton' for interpreter, uses robot telemetry to wait for button press
/// on a computer.
class WaitGamepadButtonBlock : public kitBase::blocksBase::common::WaitBlock
{
public:
	explicit WaitGamepadButtonBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private slots:
	/// Called when new data about button state is received.
	void responseSlot(int isPressed);

	void timerTimeout() override;

private:
	void run() override;

	kitBase::robotModel::DeviceInfo device() const override;

	const QString mPort;
	robotModel::parts::TrikGamepadButton *mButton;  // Does not have ownership.
};

}
}
}
