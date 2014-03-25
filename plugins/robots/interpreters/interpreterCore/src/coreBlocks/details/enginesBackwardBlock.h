#pragma once

#include "engineCommandBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class EnginesBackwardBlock : public EngineCommandBlock
{
	Q_OBJECT

public:
	explicit EnginesBackwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
