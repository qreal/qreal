#pragma once

#include "trikDisplayWidget.h"
#include "interpreterBase/blocksBase/block.h"
#include "interpreterBase/robotModel/robotModelInterface.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

/// An interpreter`s implementation for SAY block.
class SayBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit SayBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~SayBlock() override;
	void run() override;

private:
	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
