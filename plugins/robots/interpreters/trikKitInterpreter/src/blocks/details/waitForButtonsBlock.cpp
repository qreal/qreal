//#include "waitForButtonsBlock.h"
//#include "../robotParts/robotModel.h"

//using namespace trikKitInterpreter::blocks::details;
//using namespace interpreterBase::robotModel::robotParts;

//WaitForButtonsBlock::WaitForButtonsBlock(RobotModel * const robotModel, robotParts::Display &display)
//	: WaitBlock(robotModel)
//	, mDisplay(display)
//{
//}

//void WaitForButtonsBlock::run()
//{
//	mLeftButtonClicks = 0;
//	mRightButtonClicks = 0;
//	mUpButtonClicks = 0;
//	mDownButtonClicks = 0;
//	mDownLeftButtonClicks = 0;
//	mAttachButtonClicks = 0;
//	mOnButtonClicks = 0;

//	mLeftWasDown = false;
//	mRightWasDown = false;
//	mUpWasDown = false;
//	mDownWasDown = false;
//	mDownLeftWasDown = false;
//	mAttachWasDown = false;
//	mOnWasDown = false;

//	connect(mDisplay.displayImpl(), SIGNAL(response(bool,bool,bool,bool,bool,bool,bool))
//			, this, SLOT(responseSlot(bool,bool,bool,bool,bool,bool,bool)));

//	mDisplay.read();
//	mActiveWaitingTimer.start();
//}

//void WaitForButtonsBlock::timerTimeout()
//{
//	mDisplay.read();
//}

//QString WaitForButtonsBlock::name() const
//{
//	return tr("Buttons");
//}

//void WaitForButtonsBlock::responseSlot(bool leftIsDown, bool rightIsDown
//		, bool upIsDown, bool downIsDown, bool downLeftIsDown
//		, bool attachIsDown, bool onIsDown)
//{
//	clicksCounter(mLeftWasDown, leftIsDown, mLeftButtonClicks);
//	clicksCounter(mRightWasDown, rightIsDown, mRightButtonClicks);
//	clicksCounter(mUpWasDown, upIsDown, mUpButtonClicks);
//	clicksCounter(mDownWasDown, downIsDown, mDownButtonClicks);
//	clicksCounter(mDownLeftWasDown, downLeftIsDown, mDownLeftButtonClicks);
//	clicksCounter(mAttachWasDown, attachIsDown, mAttachButtonClicks);
//	clicksCounter(mOnWasDown, onIsDown, mOnButtonClicks);

//	int const targetLeftButtonClicks = evaluate("LeftButtonClicks").toInt();
//	int const targetRightButtonClicks = evaluate("RightButtonClicks").toInt();
//	int const targetUpButtonClicks = evaluate("UpButtonClicks").toInt();
//	int const targetDownButtonClicks = evaluate("DownButtonClicks").toInt();
//	int const targetDownLeftButtonClicks = evaluate("DownLeftButtonClicks").toInt();
//	int const targetAttachButtonClicks = evaluate("AttachButtonClicks").toInt();
//	int const targetOnButtonClicks = evaluate("OnButtonClicks").toInt();

//	if(mLeftButtonClicks >= targetLeftButtonClicks
//		&& mRightButtonClicks >= targetRightButtonClicks
//		&& mUpButtonClicks >= targetUpButtonClicks
//		&& mDownButtonClicks >= targetDownButtonClicks
//		&& mDownLeftButtonClicks >= targetDownLeftButtonClicks
//		&& mAttachButtonClicks >= targetAttachButtonClicks
//		&& mOnButtonClicks >= targetOnButtonClicks)
//	{
//		stop();
//	}
//}

//void WaitForButtonsBlock::clicksCounter(bool &buttonWasDown, bool buttonIsDown, int &clicks)
//{
//	if (buttonIsDown) {
//		buttonWasDown = true;
//	}

//	if (buttonWasDown && !buttonIsDown) {
//		clicks++;
//		buttonWasDown = false;
//	}
//}



