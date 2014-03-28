#pragma once

#include "engineCommandBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class EnginesForwardBlock : public EngineCommandBlock
{
	Q_OBJECT

public:
	explicit EnginesForwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
