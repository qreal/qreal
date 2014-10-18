#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class SetBackgroundBlock : public interpreterBase::blocksBase::Block
{
public:
	explicit SetBackgroundBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void run() override;

	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
