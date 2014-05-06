#pragma once

#include <interpreterBase/blocksBase/common/waitBlock.h>

namespace nxtKitInterpreter {

namespace robotModel {
namespace parts {
class NxtButtons;
}
}

namespace blocks {
namespace details {

class WaitForButtonsBlock : public interpreterBase::blocksBase::common::WaitBlock
{
	Q_OBJECT

public:
	explicit WaitForButtonsBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForButtonsBlock() override;

	void run() override;

protected slots:
	void responseSlot(bool leftIsDown, bool rightIsDown, bool centralIsDown, bool bottomIsDown);
	void timerTimeout() override;

private:
	void clicksCounter(bool &buttonWasDown, bool buttonIsDown, int &clicks);
	interpreterBase::robotModel::DeviceInfo device() const override;

	int mLeftButtonClicks;
	int mRightButtonClicks;
	int mCentralButtonClicks;
	int mBottomButtonClicks;

	bool mLeftWasDown;
	bool mRightWasDown;
	bool mCentralWasDown;
	bool mBottomWasDown;

	robotModel::parts::NxtButtons *mButtons;  // Does not take ownership
};

}
}
}
