#pragma once

#include <kitBase/blocksBase/common/deviceBlock.h>
#include "trikKit/robotModel/parts/trikShell.h"

namespace trik {
namespace blocks {
namespace details {

/// An interpreter`s implementation for "System Command" block.
class SystemCommandBlock : public kitBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikShell>
{
	Q_OBJECT

public:
	explicit SystemCommandBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::TrikShell &shell) override;
};

}
}
}
