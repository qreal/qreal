#pragma once

#include "interpreterBase/blocksBase/block.h"
#include "interpreterBase/robotModel/robotModelInterface.h"

namespace interpreterBase {

namespace robotModel {
namespace robotParts {
class Display;
}
}

namespace blocksBase {
namespace common {

/// A base for all blocks that work with robot`s display.
class ROBOTS_INTERPRETER_BASE_EXPORT DisplayBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit DisplayBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	virtual ~DisplayBlock() override;

	void run() override;

protected:
	/// Implementation may consider that display is configured and ready to work
	virtual void doJob(robotModel::robotParts::Display &display) = 0;

private:
	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
