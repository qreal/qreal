#pragma once

#include <interpreterBase/blocksBase/common/deviceBlock.h>
#include "robotModel/parts/trikShell.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

/// An interpreter`s implementation for "System Command" block.
class SystemCommandBlock : public interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikShell>
{
	Q_OBJECT

public:
	explicit SystemCommandBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~SystemCommandBlock() override;

private:
	void doJob(robotModel::parts::TrikShell &shell) override;
};

}
}
}
