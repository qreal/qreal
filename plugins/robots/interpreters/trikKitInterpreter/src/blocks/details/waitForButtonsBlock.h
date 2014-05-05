//#pragma once

//#include <interpreterBase/blocksBase/common/waitBlock.h>

//#include "src/robotModel/trikRobotModelBase.h"

//namespace trikKitInterpreter {
//namespace blocks {
//namespace details {

//class WaitForButtonsBlock : public WaitBlock
//{
//	Q_OBJECT
//public:
//	WaitForButtonsBlock(RobotModel * const robotModel, robotParts::Display &display);
//	virtual ~WaitForButtonsBlock() {}
//	virtual void run();

//protected:
//	virtual QString name() const;

//protected slots:
//	virtual void responseSlot(bool leftIsDown, bool rightIsDown
//			, bool upIsDown, bool downIsDown, bool downLeftIsDown
//			, bool attachIsDown, bool onIsDown);

//	virtual void timerTimeout();

//private:
//	void clicksCounter(bool &buttonWasDown, bool buttonIsDown, int &clicks);

//	int mLeftButtonClicks;
//	int mRightButtonClicks;
//	int mUpButtonClicks;
//	int mDownButtonClicks;
//	int mDownLeftButtonClicks;
//	int mAttachButtonClicks;
//	int mOnButtonClicks;


//	bool mLeftWasDown;
//	bool mRightWasDown;
//	bool mUpWasDown;
//	bool mDownWasDown;
//	bool mDownLeftWasDown;
//	bool mAttachWasDown;
//	bool mOnWasDown;

//	robotParts::Display &mDisplay;
//};

//}
//}
//}
