#pragma once

#include "speakerBlock.h"

namespace nxt {
namespace blocks {
namespace details {

class PlayToneBlock : public SpeakerBlock
{
	Q_OBJECT

public:
	explicit PlayToneBlock(kitBase::robotModel::RobotModelInterface &robotModel);

protected:
	int duration() override;
	void doJob(robotModel::parts::NxtSpeaker &speaker) override;
};

}
}
}
