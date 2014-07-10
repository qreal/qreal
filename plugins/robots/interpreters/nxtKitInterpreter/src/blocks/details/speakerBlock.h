#pragma once

#include <interpreterBase/blocksBase/block.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

#include "robotModel/parts/nxtSpeaker.h"

namespace utils {
class AbstractTimer;
}

namespace nxtKitInterpreter {
namespace blocks {
namespace details {

class SpeakerBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit SpeakerBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	virtual ~SpeakerBlock() override;

	void run() override;

protected:
	virtual int duration() = 0;

	/// Implementation may consider that speaker is configured and ready to work
	virtual void doJob(robotModel::parts::NxtSpeaker &speaker) = 0;

private slots:
	void timeout();

private:
	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
	utils::AbstractTimer * const mTimer;  // Takes ownership
};

}
}
}
