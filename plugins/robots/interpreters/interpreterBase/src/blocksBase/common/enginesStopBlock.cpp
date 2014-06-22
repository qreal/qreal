#include "interpreterBase/blocksBase/common/enginesStopBlock.h"

#include <interpreterBase/robotModel/robotParts/motor.h>

using namespace interpreterBase::blocksBase::common;
using namespace interpreterBase::robotModel;

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
