#include <interpreterBase/blocksBase/common/waitForButtonBlock.h>

#include <interpreterBase/robotModel/robotModelInterface.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

using namespace interpreterBase::blocksBase::common;
using namespace interpreterBase::robotModel;

WaitForButtonBlock::WaitForButtonBlock(robotModel::RobotModelInterface &robotModel)
	: WaitBlock(robotModel)
{
}

void WaitForButtonBlock::run()
{
	const QString port = eval<QString>("Button");

	mButton = RobotModelUtils::findDevice<robotParts::Button>(mRobotModel, port);

	if (!mButton) {
		mActiveWaitingTimer.stop();
		error(tr("Incorrect button port %1").arg(port));
		return;
	}

	connect(mButton, &robotModel::robotParts::Button::newData, this, &WaitForButtonBlock::responseSlot);

	mButton->read();
	mActiveWaitingTimer.start();
}

void WaitForButtonBlock::timerTimeout()
{
	mButton->read();
}

void WaitForButtonBlock::responseSlot(int isPressed)
{
	if (isPressed) {
		stop();
	}
}

DeviceInfo WaitForButtonBlock::device() const
{
	return DeviceInfo::create<robotModel::robotParts::Button>();
}
