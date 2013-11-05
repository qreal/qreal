#pragma once

#include "waitBlock.h"
#include "../robotParts/robotModel.h"

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace details
{
namespace blocks
{

class WaitForButtonsBlock : public WaitBlock
{
	Q_OBJECT
public:
	WaitForButtonsBlock(RobotModel * const robotModel, robotParts::Display &display);
	virtual ~WaitForButtonsBlock() {}
	virtual void run();

protected:
	virtual QString name() const;

protected slots:
	virtual void responseSlot(bool leftIsDown, bool rightIsDown
			, bool upIsDown, bool downIsDown, bool downLeftIsDown
			, bool attachIsDown, bool onIsDown);

	virtual void timerTimeout();

private:
	void clicksCounter(bool &buttonWasDown, bool buttonIsDown, int &clicks);

	int mLeftButtonClicks;
	int mRightButtonClicks;
	int mUpButtonClicks;
	int mDownButtonClicks;
	int mDownLeftButtonClicks;
	int mAttachButtonClicks;
	int mOnButtonClicks;


	bool mLeftWasDown;
	bool mRightWasDown;
	bool mUpWasDown;
	bool mDownWasDown;
	bool mDownLeftWasDown;
	bool mAttachWasDown;
	bool mOnWasDown;

	robotParts::Display &mDisplay;
};

}
}
}
}
}
