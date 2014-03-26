#include "enginesStopBlock.h"

#include <interpreterBase/robotModel/robotParts/motor.h>

using namespace interpreterCore::coreBlocks::details;

EnginesStopBlock::EnginesStopBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void EnginesStopBlock::run()
{
	for (interpreterBase::robotModel::robotParts::Motor * const motor : parsePorts()) {
		motor->off();
	}

	emit done(mNextBlockId);
}
