#include "trikEnginesForwardBlock.h"

#include <kitBase/robotModel/robotParts/motor.h>

using namespace trik::blocks::details;
using namespace kitBase::robotModel::robotParts;

TrikEnginesForwardBlock::TrikEnginesForwardBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::EngineCommandBlock(robotModel)
{
}

void TrikEnginesForwardBlock::run()
{
	const auto result = eval<int>("Power");
	if (!errorsOccured()) {
		for (Motor * const motor : parsePorts<Motor>()) {
			motor->on(result);
		}

		emit done(mNextBlockId);
	}
}
