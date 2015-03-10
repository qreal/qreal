#pragma once

#include "kitBase/blocksBase/common/deviceBlock.h"
#include "trikKit/robotModel/parts/trikLed.h"

namespace trik {
namespace blocks {
namespace details {

/// An interpreter`s implementation for LED block.
/// sets LED widget's color.
class LedBlock : public kitBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikLed>
{
	Q_OBJECT

public:
	explicit LedBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::TrikLed &led) override;
};

}
}
}
