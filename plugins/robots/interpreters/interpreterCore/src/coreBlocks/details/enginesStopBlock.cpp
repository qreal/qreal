#include "enginesStopBlock.h"

#include <interpreterBase/robotModel/robotParts/motor.h>

using namespace interpreterCore::coreBlocks::details;
using namespace interpreterBase::blocksBase::common;
using namespace interpreterBase::robotModel::robotParts;

EnginesStopBlock::EnginesStopBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void EnginesStopBlock::run()
{
	for (Motor * const motor : parsePorts<Motor>()) {
		motor->off();
	}

	emit done(mNextBlockId);
}
