#pragma once

#include "speakerBlock.h"

namespace nxtKitInterpreter {
namespace blocks {
namespace details {

class BeepBlock : public SpeakerBlock
{
	Q_OBJECT

public:
	explicit BeepBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~BeepBlock() override;

protected:
	int duration() override;
	void doJob(robotModel::parts::NxtSpeaker &speaker) override;
};

}
}
}
