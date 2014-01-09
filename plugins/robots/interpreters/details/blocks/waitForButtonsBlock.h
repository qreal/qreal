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
	virtual void responseSlot(bool leftIsDown, bool rightIsDown, bool centralIsDown, bool bottomIsDown);
	virtual void timerTimeout();

private:
	void clicksCounter(bool &buttonWasDown, bool buttonIsDown, int &clicks);

	int mLeftButtonClicks;
	int mRightButtonClicks;
	int mCentralButtonClicks;
	int mBottomButtonClicks;

	bool mLeftWasDown;
	bool mRightWasDown;
	bool mCentralWasDown;
	bool mBottomWasDown;

	robotParts::Display &mDisplay;
};

}
}
}
}
}
