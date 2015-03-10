#pragma once

#include <kitBase/blocksBase/common/deviceBlock.h>
#include "trikKit/robotModel/parts/trikShell.h"

namespace trik {
namespace blocks {
namespace details {

/// An interpreter`s implementation for SAY block.
class SayBlock : public kitBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikShell>
{
	Q_OBJECT

public:
	explicit SayBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::TrikShell &shell) override;
};

}
}
}
