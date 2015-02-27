#include "kitBase/blocksBase/common/enginesStopBlock.h"

#include <kitBase/robotModel/robotParts/motor.h>

using namespace kitBase::blocksBase::common;
using namespace kitBase::robotModel;

EnginesStopBlock::EnginesStopBlock(RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void EnginesStopBlock::run()
{
	for (robotParts::Motor * const motor : parsePorts<robotParts::Motor>()) {
		motor->off();
	}

	emit done(mNextBlockId);
}
