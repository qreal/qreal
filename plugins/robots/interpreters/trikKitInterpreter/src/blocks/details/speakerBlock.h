#pragma once

#include <interpreterBase/blocksBase/common/deviceBlock.h>
#include "robotModel/parts/trikSpeaker.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

/// An interpreter`s implementation for speaker block.
class SpeakerBlock : public interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikSpeaker>
{
	Q_OBJECT

public:
	explicit SpeakerBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::TrikSpeaker &shell) override;
};

}
}
}
