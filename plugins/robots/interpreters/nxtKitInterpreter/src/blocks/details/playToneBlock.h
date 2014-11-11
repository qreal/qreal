#pragma once

#include "speakerBlock.h"

namespace nxtKitInterpreter {
namespace blocks {
namespace details {

class PlayToneBlock : public SpeakerBlock
{
	Q_OBJECT

public:
	explicit PlayToneBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

protected:
	int duration() override;
	void doJob(robotModel::parts::NxtSpeaker &speaker) override;
};

}
}
}
