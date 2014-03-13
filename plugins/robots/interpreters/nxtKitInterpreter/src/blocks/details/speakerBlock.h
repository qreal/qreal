#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include <interpreterBase/blocksBase/block.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

/// @todo: use common speaker interface
#include "robotModel/real/parts/speaker.h"

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
	virtual void doJob(robotModel::real::parts::Speaker &speaker) = 0;

private slots:
	void timeout();

private:
	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
	utils::AbstractTimer * const mTimer;  // Takes ownership
};

}
}
}
