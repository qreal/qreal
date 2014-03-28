#include "waitForButtonsBlock.h"

#include <interpreterBase/robotModel/robotModelUtils.h>

#include "robotModel/parts/nxtButtons.h"

using namespace nxtKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel;

WaitForButtonsBlock::WaitForButtonsBlock(RobotModelInterface &robotModel)
	: WaitBlock(robotModel)
	, mButtons(nullptr)
{
}

WaitForButtonsBlock::~WaitForButtonsBlock()
{
}

void WaitForButtonsBlock::run()
{
	mLeftButtonClicks = 0;
	mRightButtonClicks = 0;
	mCentralButtonClicks = 0;
	mBottomButtonClicks = 0;

	mLeftWasDown = false;
	mRightWasDown = false;
	mCentralWasDown = false;
	mBottomWasDown = false;

	QString const port = "ButtonsPort";
	mButtons = RobotModelUtils::findDevice<robotModel::parts::NxtButtons>(mRobotModel, port);

	if (!mButtons) {
		mActiveWaitingTimer.stop();
		error(tr("Buttons are not configured (WTF?)"));
		return;
	}


	connect(mButtons, &robotModel::parts::NxtButtons::response, this, &WaitForButtonsBlock::responseSlot);

	mButtons->read();
	mActiveWaitingTimer.start();
}

void WaitForButtonsBlock::timerTimeout()
{
	mButtons->read();
}

void WaitForButtonsBlock::responseSlot(bool leftIsDown, bool rightIsDown, bool centralIsDown, bool bottomIsDown)
{
	clicksCounter(mLeftWasDown, leftIsDown, mLeftButtonClicks);
	clicksCounter(mRightWasDown, rightIsDown, mRightButtonClicks);
	clicksCounter(mCentralWasDown, centralIsDown, mCentralButtonClicks);
	clicksCounter(mBottomWasDown, bottomIsDown, mBottomButtonClicks);

	int const targetLeftButtonClicks = evaluate("LeftButtonClicks").toInt();
	int const targetRightButtonClicks = evaluate("RightButtonClicks").toInt();
	int const targetCentralButtonClicks = evaluate("CentralButtonClicks").toInt();
	int const targetBottomButtonClicks = evaluate("BottomButtonClicks").toInt();

	if(mLeftButtonClicks >= targetLeftButtonClicks
		&& mRightButtonClicks >= targetRightButtonClicks
		&& mCentralButtonClicks >= targetCentralButtonClicks
		&& mBottomButtonClicks >= targetBottomButtonClicks) {
		stop();
	}
}

void WaitForButtonsBlock::clicksCounter(bool &buttonWasDown, bool buttonIsDown, int &clicks)
{
	if (buttonIsDown) {
		buttonWasDown = true;
	}

	if (buttonWasDown && !buttonIsDown) {
		clicks++;
		buttonWasDown = false;
	}
}
