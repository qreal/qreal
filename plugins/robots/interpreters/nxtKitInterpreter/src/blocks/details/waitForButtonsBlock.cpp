#include "waitForButtonsBlock.h"

#include <interpreterBase/robotModel/robotModelUtils.h>

#include "robotModel/parts/nxtDisplay.h"

using namespace nxtKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel;

WaitForButtonsBlock::WaitForButtonsBlock(RobotModelInterface &robotModel)
	: WaitBlock(robotModel)
	, mDisplay(nullptr)
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

	QString const port = "DisplayPort";
	mDisplay = RobotModelUtils::findDevice<robotModel::parts::NxtDisplay>(mRobotModel, port);

	if (!mDisplay) {
		mActiveWaitingTimer.stop();
		error(tr("Display is not configured (WTF?)"));
		return;
	}


	/// @todo: create buttons device and use it jedi
//	connect(mDisplay, &robotModel::parts::NxtDisplay, this, SLOT(responseSlot(bool,bool,bool,bool)));

//	mDisplay.read();
	mActiveWaitingTimer.start();
}

void WaitForButtonsBlock::timerTimeout()
{
//	mDisplay.read();
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
