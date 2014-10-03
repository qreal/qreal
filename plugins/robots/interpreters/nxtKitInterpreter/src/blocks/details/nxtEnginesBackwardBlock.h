#pragma once

#include <interpreterBase/blocksBase/common/engineCommandBlock.h>

namespace nxtKitInterpreter {
namespace blocks {
namespace details {

class NxtEnginesBackwardBlock : public interpreterBase::blocksBase::common::EngineCommandBlock
{
	Q_OBJECT

public:
	explicit NxtEnginesBackwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
