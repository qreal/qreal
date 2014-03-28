#pragma once

#include "engineCommandBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class EnginesStopBlock : public EngineCommandBlock
{
	Q_OBJECT

public:
	explicit EnginesStopBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
