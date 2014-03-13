#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "speakerBlock.h"

namespace nxtKitInterpreter {
namespace blocks {
namespace details {

class PlayToneBlock : public SpeakerBlock
{
	Q_OBJECT

public:
	explicit PlayToneBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~PlayToneBlock() override;

protected:
	int frequency();
	int duration() override;
	void doJob(robotModel::real::parts::Speaker &speaker) override;
};

}
}
}
