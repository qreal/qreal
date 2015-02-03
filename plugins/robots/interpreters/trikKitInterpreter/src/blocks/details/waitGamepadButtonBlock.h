#pragma once

#include <interpreterBase/blocksBase/common/waitForButtonBlock.h>

namespace trikKitInterpreter {
namespace blocks {
namespace details {

/// Implementation of 'TrikWaitGamepadButton' for interpreter, uses robot telemetry to wait for button press
/// on a computer.
class WaitGamepadButtonBlock : public interpreterBase::blocksBase::common::WaitForButtonBlock
{
public:
	explicit WaitGamepadButtonBlock(interpreterBase::robotModel::RobotModelInterface &robotModel
			, const QString &portName);
};

}
}
}
