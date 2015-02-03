#include "waitGamepadButtonBlock.h"

#include "robotModel/parts/trikGamepadButton.h"

using namespace trikKitInterpreter::blocks::details;

WaitGamepadButtonBlock::WaitGamepadButtonBlock(interpreterBase::robotModel::RobotModelInterface &robotModel
		, const QString &portName)
	: WaitForButtonBlock(robotModel, portName)
{
}
