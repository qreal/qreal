#include "waitForButtonsBlock.h"
#include "../robotParts/robotModel.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace qReal::interpreters::robots::details::blocks;

WaitForButtonsBlock::WaitForButtonsBlock(RobotModel * const robotModel, robotParts::Display &display)
	: WaitBlock(robotModel)
	, mDisplay(display)
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

	connect(mDisplay.displayImpl(), SIGNAL(response(bool,bool,bool,bool)), this, SLOT(responseSlot(bool,bool,bool,bool)));

	mDisplay.read();
	mActiveWaitingTimer.start();
}

void WaitForButtonsBlock::timerTimeout()
{
	mDisplay.read();
}

QString WaitForButtonsBlock::name() const
{
	return tr("Display");
}

void WaitForButtonsBlock::responseSlot(bool leftIsDown, bool rightIsDown, bool centralIsDown, bool bottomIsDown)
{
	if(leftIsDown){
		mLeftWasDown = true;
	}
	if(mLeftWasDown && !leftIsDown){
		mLeftButtonClicks++;
		mLeftWasDown = false;
	}

	if(rightIsDown){
		mRightWasDown = true;
	}
	if(mRightWasDown && !rightIsDown){
		mRightButtonClicks++;
		mRightWasDown = false;
	}
	if(centralIsDown){
		mCentralWasDown = true;
	}
	if(mCentralWasDown && !centralIsDown){
		mCentralButtonClicks++;
		mCentralWasDown = false;
	}

	if(bottomIsDown){
		mBottomWasDown = true;
	}
	if(mBottomWasDown && !bottomIsDown){
		mBottomButtonClicks++;
		mBottomWasDown = false;
	}


	int const targetLeftButtonClicks = evaluate("LeftButtonClicks").toInt();
	int const targetRightButtonClicks = evaluate("RightButtonClicks").toInt();
	int const targetCentralButtonClicks = evaluate("CentralButtonClicks").toInt();
	int const targetBottomButtonClicks = evaluate("BottomButtonClicks").toInt();

	if(mLeftButtonClicks >= targetLeftButtonClicks
		&& mRightButtonClicks >= targetRightButtonClicks
		&& mCentralButtonClicks >= targetCentralButtonClicks
		&& mBottomButtonClicks >= targetBottomButtonClicks){
		stop();
	}
}



