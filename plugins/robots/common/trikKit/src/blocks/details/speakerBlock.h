#pragma once

#include <kitBase/blocksBase/common/deviceBlock.h>
#include "trikKit/robotModel/parts/trikSpeaker.h"

namespace trik {
namespace blocks {
namespace details {

/// An interpreter`s implementation for speaker block.
class SpeakerBlock : public kitBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikSpeaker>
{
	Q_OBJECT

public:
	explicit SpeakerBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::TrikSpeaker &shell) override;
};

}
}
}
