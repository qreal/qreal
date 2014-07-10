#pragma once

#include <interpreterBase/blocksBase/common/engineCommandBlock.h>

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class TrikEnginesBackwardBlock : public interpreterBase::blocksBase::common::EngineCommandBlock
{
	Q_OBJECT

public:
	explicit TrikEnginesBackwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
