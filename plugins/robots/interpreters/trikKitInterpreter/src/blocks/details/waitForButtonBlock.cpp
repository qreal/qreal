#include "waitForButtonBlock.h"

#include <interpreterBase/robotModel/robotModelInterface.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel::robotParts;

WaitForButtonsBlock::WaitForButtonsBlock(
		interpreterBase::robotModel::RobotModelInterface &robotModel
		, TrikButtonsEnum button)
	: WaitBlock(robotModel)
	, mButton(button)
{
}

void WaitForButtonsBlock::run()
{
	mButtons = interpreterBase::robotModel::RobotModelUtils::findDevice<robotModel::parts::TrikButton>(
					mRobotModel, "ButtonsPort"
			);

	if (!mButtons) {
		mActiveWaitingTimer.stop();
		error(tr("Buttons are not configured (WTF?)"));
		return;
	}

	connect(mButtons, &robotModel::parts::TrikButton::response, this, &WaitForButtonsBlock::responseSlot);

	mButtons->read();
	mActiveWaitingTimer.start();
}

void WaitForButtonsBlock::timerTimeout()
{
	mButtons->read();
}

QString WaitForButtonsBlock::name() const
{
	return tr("Buttons");
}

void WaitForButtonsBlock::responseSlot(bool leftIsPressed
		, bool rightIsPressed
		, bool upIsPressed
		, bool downIsPressed
		, bool enterIsPresed
		, bool escapeIsPresed
		, bool powerIsPressed)
{
	switch (mButton) {
	case left:
		if (leftIsPressed) {
			stop();
		}
	case right:
		if (rightIsPressed) {
			stop();
		}
	case up:
		if (upIsPressed) {
			stop();
		}
	case down:
		if (downIsPressed) {
			stop();
		}
	case enter:
		if (enterIsPresed) {
			stop();
		}
	case escape:
		if (escapeIsPresed) {
			stop();
		}
	case power:
		if (powerIsPressed) {
			stop();
		}
	}
}

interpreterBase::robotModel::DeviceInfo WaitForButtonsBlock::device() const
{
	return interpreterBase::robotModel::DeviceInfo::create<robotModel::parts::TrikButton>();
}
