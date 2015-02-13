#include "waitGamepadButtonBlock.h"

#include <interpreterBase/robotModel/robotModelInterface.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

#include "robotModel/parts/trikGamepadButton.h"

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel;

WaitGamepadButtonBlock::WaitGamepadButtonBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitBlock(robotModel)
{
	mActiveWaitingTimer.setInterval(200);
}

void WaitGamepadButtonBlock::run()
{
	const QString port = eval<QString>("Button");
	const QString portName = QString("GamepadButton%1Port").arg(port);

	mButton = RobotModelUtils::findDevice<robotModel::parts::TrikGamepadButton>(mRobotModel, portName);

	if (!mButton) {
		mActiveWaitingTimer.stop();
		error(tr("Incorrect port for gamepad button %1").arg(port));
		return;
	}

	connect(mButton, &robotModel::parts::TrikGamepadButton::newData, this, &WaitGamepadButtonBlock::responseSlot);

	mButton->read();
	mActiveWaitingTimer.start();
}

void WaitGamepadButtonBlock::timerTimeout()
{
	mButton->read();
}

void WaitGamepadButtonBlock::responseSlot(int isPressed)
{
	if (isPressed) {
		stop();
	}
}

DeviceInfo WaitGamepadButtonBlock::device() const
{
	return DeviceInfo::create<robotModel::parts::TrikGamepadButton>();
}
