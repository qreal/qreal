#pragma once

#include <interpreterBase/blocksBase/block.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

#include "robotModel/parts/nxtDisplay.h"

namespace nxtKitInterpreter {
namespace blocks {
namespace details {

class DisplayBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit DisplayBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	virtual ~DisplayBlock() override;

	void run() override;

protected:
	/// Implementation may consider that display is configured and ready to work
	virtual void doJob(robotModel::parts::NxtDisplay &display) = 0;

private:
	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
