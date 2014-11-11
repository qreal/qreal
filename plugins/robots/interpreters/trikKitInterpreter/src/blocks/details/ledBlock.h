#pragma once

#include "interpreterBase/blocksBase/common/deviceBlock.h"
#include "src/robotModel/parts/trikLed.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

/// An interpreter`s implementation for LED block.
/// sets LED widget's color.
class LedBlock : public interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikLed>
{
	Q_OBJECT

public:
	explicit LedBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::TrikLed &led) override;
};

}
}
}
