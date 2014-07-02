#pragma once

#include <interpreterBase/blocksBase/common/engineCommandBlock.h>

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class TrikEnginesForwardBlock : public interpreterBase::blocksBase::common::EngineCommandBlock
{
	Q_OBJECT

public:
	explicit TrikEnginesForwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
