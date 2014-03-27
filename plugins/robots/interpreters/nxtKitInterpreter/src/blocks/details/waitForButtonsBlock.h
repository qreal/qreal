#pragma once

#include "waitBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForButtonsBlock : public WaitBlock
{
	Q_OBJECT
public:
	explicit WaitForButtonsBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForButtonsBlock() override;

	void run() override;

protected:
	QString name() const override;

protected slots:
	void responseSlot(bool leftIsDown, bool rightIsDown, bool centralIsDown, bool bottomIsDown);
	void timerTimeout() override;

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
