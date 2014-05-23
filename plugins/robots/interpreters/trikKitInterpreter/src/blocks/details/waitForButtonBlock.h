#pragma once

#include <interpreterBase/blocksBase/common/waitBlock.h>

#include "src/robotModel/trikRobotModelBase.h"
#include "src/robotModel/parts/trikButton.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class WaitForButtonsBlock : public interpreterBase::blocksBase::common::WaitBlock
{
	Q_OBJECT

public:
	enum TrikButtonsEnum {
		left
		, right
		, up
		, down
		, enter
		, escape
		, power
	};

	WaitForButtonsBlock(interpreterBase::robotModel::RobotModelInterface &robotModel, TrikButtonsEnum button);
	virtual ~WaitForButtonsBlock() {}

protected:
	virtual QString name() const;

protected slots:
	void responseSlot(bool leftIsPressed
			, bool rightIsPressed
			, bool upIsPressed
			, bool downIsPressed
			, bool enterIsPresed
			, bool escapeIsPresed
			, bool powerIsPressed);

	virtual void timerTimeout();

private:
	void run() override;
	interpreterBase::robotModel::DeviceInfo device() const override;

	TrikButtonsEnum mButton;

	robotModel::parts::TrikButton *mButtons;
};

}
}
}
