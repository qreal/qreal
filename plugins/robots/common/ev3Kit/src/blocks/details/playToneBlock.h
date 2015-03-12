#pragma once

#include "speakerBlock.h"

namespace ev3 {
namespace blocks {
namespace details {

class PlayToneBlock : public SpeakerBlock
{
	Q_OBJECT

public:
	explicit PlayToneBlock(kitBase::robotModel::RobotModelInterface &robotModel);

protected:
	int duration() override;
	void doJob(robotModel::parts::Ev3Speaker &speaker) override;
};

}
}
}
