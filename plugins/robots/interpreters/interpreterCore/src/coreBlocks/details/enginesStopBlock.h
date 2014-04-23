#pragma once

#include <interpreterBase/blocksBase/common/engineCommandBlock.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class EnginesStopBlock : public interpreterBase::blocksBase::common::EngineCommandBlock
{
	Q_OBJECT

public:
	explicit EnginesStopBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
