#include <interpreterBase/blocksBase/common/waitForButtonBlock.h>

#include <interpreterBase/robotModel/robotModelInterface.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

using namespace interpreterBase::blocksBase::common;
using namespace interpreterBase::robotModel;

WaitForButtonBlock::WaitForButtonBlock(RobotModelInterface &robotModel, QString const &portName)
	: WaitBlock(robotModel)
	, mPort(portName)
{
}

WaitForButtonBlock::~WaitForButtonBlock()
{
}

void WaitForButtonBlock::run()
{
	mButton = RobotModelUtils::findDevice<robotParts::Button>(mRobotModel, mPort);

	if (!mButton) {
		mActiveWaitingTimer.stop();
		error(tr("Button on port %1 is not configured (WTF?)").arg(mPort));
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
