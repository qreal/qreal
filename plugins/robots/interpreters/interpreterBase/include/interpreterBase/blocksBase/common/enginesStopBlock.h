#pragma once

#include <interpreterBase/blocksBase/common/engineCommandBlock.h>

namespace interpreterBase {
namespace blocksBase {
namespace common {

class ROBOTS_INTERPRETER_BASE_EXPORT EnginesStopBlock : public interpreterBase::blocksBase::common::EngineCommandBlock
{
	Q_OBJECT

public:
	explicit EnginesStopBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
