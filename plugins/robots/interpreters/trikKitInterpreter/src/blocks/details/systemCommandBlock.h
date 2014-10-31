#pragma once

#include "trikDisplayWidget.h"
#include "interpreterBase/blocksBase/block.h"
#include "interpreterBase/robotModel/robotModelInterface.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

/// An interpreter`s implementation for "System Command" block.
class SystemCommandBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit SystemCommandBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~SystemCommandBlock() override;
	void run() override;

private:
	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
