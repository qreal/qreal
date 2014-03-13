#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

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
	void doJob(robotModel::real::parts::Speaker &speaker) override;
};

}
}
}
