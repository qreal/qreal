#pragma once

#include <interpreterBase/blocksBase/common/waitBlock.h>
#include <interpreterBase/robotModel/robotParts/button.h>

namespace interpreterBase {
namespace blocksBase {
namespace common {

/// An interpreter`s implementation for the buttons wait block.
class ROBOTS_INTERPRETER_BASE_EXPORT WaitForButtonBlock : public interpreterBase::blocksBase::common::WaitBlock
{
	Q_OBJECT

public:
	WaitForButtonBlock(interpreterBase::robotModel::RobotModelInterface &robotModel, QString const &portName);
	~WaitForButtonBlock() override;

protected slots:
	void responseSlot(int isPressed);
	void timerTimeout() override;

private:
	void run() override;
	interpreterBase::robotModel::DeviceInfo device() const override;

	QString const mPort;
	robotModel::robotParts::Button *mButton;
};

}
}
}
