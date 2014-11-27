#pragma once

#include <interpreterBase/blocksBase/common/deviceBlock.h>
#include "robotModel/parts/trikShell.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

/// An interpreter`s implementation for SAY block.
class SayBlock : public interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikShell>
{
	Q_OBJECT

public:
	explicit SayBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::TrikShell &shell) override;
};

}
}
}
