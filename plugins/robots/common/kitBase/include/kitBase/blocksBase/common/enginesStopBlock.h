#pragma once

#include <kitBase/blocksBase/common/engineCommandBlock.h>

namespace kitBase {
namespace blocksBase {
namespace common {

/// An interpreter`s implementation for the motors stop block.
class ROBOTS_KIT_BASE_EXPORT EnginesStopBlock : public EngineCommandBlock
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit EnginesStopBlock(robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
