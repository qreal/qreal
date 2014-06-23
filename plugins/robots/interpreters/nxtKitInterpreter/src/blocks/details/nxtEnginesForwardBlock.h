#pragma once

#include <interpreterBase/blocksBase/common/engineCommandBlock.h>

namespace nxtKitInterpreter {
namespace blocks {
namespace details {

class NxtEnginesForwardBlock : public interpreterBase::blocksBase::common::EngineCommandBlock
{
	Q_OBJECT

public:
	explicit NxtEnginesForwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
